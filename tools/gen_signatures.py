#!/usr/bin/env python3

import argparse
import re
import subprocess

from dataclasses import dataclass
from pathlib import Path

from get_platform import get_platform

# Runs `dsd sig new` on all functions from a `symbols.txt` file.
# Example of usage: `python tools/gen_signatures.py -i -o 61 eur` to get data for overlay 61's symbols.
# Place this file in a subfolder from your project (for instance `tools`)


INDENT = " " * 4

parser = argparse.ArgumentParser(description="Generates build.ninja")
parser.add_argument("--dsd", type=Path, required=False, help="Path to pre-installed dsd CLI")
parser.add_argument("-o", "--overlay", type=int, required=False, help="Overlay ID (defaults to main symbols if unset)")
parser.add_argument("-i", "--ignore-unnamed", action="store_true", required=False, help="Ignore unnamed functions")
parser.add_argument("-r", "--check-relocs", action="store_true", required=False, help="Check if a function is relocated")
parser.add_argument('version', help='Game version')
args = parser.parse_args()

# Platform info
platform = get_platform()
if platform is None:
    exit(1)
EXE = platform.exe

current_path     = Path(__name__)
root_path        = current_path.parent
DSD = (args.dsd or root_path / f"dsd{EXE}").resolve()


# Helper classes to parse the data

@dataclass
class SignatureReloc:
    offset: int
    name: str
    kind: str


@dataclass
class Signature:
    bitmask: str
    pattern: str
    relocs: list[SignatureReloc]


@dataclass
class Signatures:
    name: str
    signatures: list[Signature]
    dsd_out: str

    @staticmethod
    def from_dsd(dsd_out: str):
        signatures: list[Signature] = []

        result = re.search(r"name:\s?([a-zA-Z_0-9]*)\n?signatures:\n?(.*)", dsd_out, re.DOTALL | re.MULTILINE)
        assert result is not None, "no base match found!"

        raw_sigs = str(result.group(2)) # dumb type hints...
        sig_results = re.finditer(
            r"-\s?bitmask:\s?([a-zA-Z0-9/+=]*)\n\s\s?pattern:\s?([a-zA-Z0-9/+=]*)\n\s\s?relocations:((?:\n\s\s?-\s?offset:\s?\d+\n\s\s?\s?\s?name:\s?[a-zA-Z0-9_]*\n?\s?\s?\s?\s?kind:\s?[a-zA-Z0-9]*)+|(\s?\[\]))",
            raw_sigs,
            re.DOTALL | re.MULTILINE,
        )

        for sig_match in sig_results:
            relocs: list[SignatureReloc] = []
            raw_relocs = str(sig_match.group(3)).removeprefix("\n")
            reloc_results = re.finditer(
                r"-\s*offset:\s*(\d+)\n\s*name:\s*([a-zA-Z0-9_]+)\n\s*kind:\s*([a-zA-Z]+)",
                raw_relocs,
                re.DOTALL | re.MULTILINE,
            )

            for reloc_match in reloc_results:
                relocs.append(SignatureReloc(int(reloc_match.group(1), base=0), reloc_match.group(2), reloc_match.group(3)))

            signatures.append(Signature(sig_match.group(1), sig_match.group(2), relocs))

        return Signatures(result.group(1), signatures, dsd_out)


def get_new_signatures(config_path: Path, function: str):
    """Returns a new signature from dsd's output"""

    if args.ignore_unnamed and function.startswith("func_"):
        return None

    out = subprocess.run([DSD, "sig", "new", "-c", config_path, "-f", function], capture_output=True, text=True)
    assert out.returncode == 0, f"Error running dsd:\n{out.stderr.strip()}"
    return Signatures.from_dsd(out.stdout)


def is_function_relocated(functions: list[Signatures], name: str):
    """Checks if the provided function is relocated somewhere, ignored if `-r` is passed"""

    if args.check_relocs:
        for function in functions:
            for sig in function.signatures:
                for reloc in sig.relocs:
                    if reloc.name == name:
                        return True

    return False


def main():
    base_path = Path("config").resolve() / str(args.version) / "arm9"
    config_path = base_path / "config.yaml"
    functions: list[Signatures] = []

    if args.overlay:
        symbols_path = base_path / "overlays" / f"ov{args.overlay:03}" / "symbols.txt"
    else:
        symbols_path = base_path / "symbols.txt"
    assert symbols_path.exists(), "symbols.txt not found!"

    symbols = symbols_path.read_text()
    func_results = re.finditer(r"([a-zA-Z0-9_]*)\s?kind:function", symbols, re.DOTALL | re.MULTILINE)

    # get the signatures
    for func_match in func_results:
        new_signature = get_new_signatures(config_path, func_match.group(1))

        if new_signature is not None and not is_function_relocated(functions, new_signature.name):
            functions.append(new_signature)
            # print(f"[INFO]: Found {new_signature.name}!")

    # write the yaml files and print what to add to dsd
    output_dir = Path("dsd_assets")
    output_dir.mkdir(exist_ok=True)

    print("Add the following to `SIGNATURES` inside `cli/src/analysus/signature.rs`:")
    for function in functions:
        output = output_dir / f"{function.name}.yaml"
        output.write_text(function.dsd_out)
        print(INDENT + f'("{function.name}", include_str!("../../../assets/signatures/{function.name}.yaml")),')


if __name__ == "__main__":
    main()
