from pathlib import Path
import subprocess

name = "seddari"
exe_path = "./seddari"

tests_main_dir = Path("tests_cases")
tests_main_dir.mkdir(parents=True, exist_ok=True)

non_interactive_tests = {
	"unknown": ["unknown"],
	"address-short": ["-a"],
	"address-long": ["--address"],
	"port-short": ["-p"],
	"port-long": ["--port"],
    "help-short": ["-h"],
    "help-long": ["--help"],
}

for i, (test_name, options) in enumerate(non_interactive_tests.items(), start=1):
    test_dir = tests_main_dir / test_name
    test_dir.mkdir(parents=True, exist_ok=True)

    with open(test_dir / "expected.out", "w") as output_file:
        subprocess.run(
            [exe_path, *options],
            stdout=output_file,
			stderr=output_file
        )

    print(f"Created {i}.{test_name} test!")
