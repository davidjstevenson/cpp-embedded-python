import sys
import os
import re
import zipfile
import shutil

DIR_PATH = os.path.dirname(os.path.realpath(__file__))
EMBEDDED_ZIP_RE = re.compile(r"python-\d\.\d.\d-embed-.*\.zip")
EMBEDDED_ZIP_PTH = re.compile(r"python\d+._pth")
INSTALL_PIP_URL = "https://bootstrap.pypa.io/get-pip.py"


def get_embedded_paths():
    path = os.path.join(DIR_PATH, "embedded-python")
    files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f)) and EMBEDDED_ZIP_RE.match(f)]

    if len(files) == 0:
        raise RuntimeError("You must download the embedded version of python and place that zip file in the 'embedded-python' folder before running this script")

    if len(files) > 1:
        raise RuntimeError("Please ensure only one version of the python embedded zip file is in the folder 'embedded-python'")

    extracted_folder = os.path.join(path, files[0])[:-4]

    pth = None
    if os.path.exists(extracted_folder):
        for f in os.listdir(extracted_folder):
            if EMBEDDED_ZIP_PTH.match(f):
                pth = f
                break

        if pth is None:
            raise RuntimeError("Could not find a 'python**._pth' file")

    return path, files[0], extracted_folder, pth


def extract_python_zip(force=False):
    path, zip_file, extracted_folder, _ = get_embedded_paths()

    if os.path.exists(extracted_folder):
        if force:
            shutil.rmtree(extracted_folder)
        else:
            return

    zip = zipfile.ZipFile(os.path.join(path, zip_file), "r")
    zip.extractall(extracted_folder)


def install_pip():
    path, zip_file, extracted_folder, pth = get_embedded_paths()
    get_pip_path = os.path.join(path, "get-pip.py")

    if not os.path.exists(get_pip_path):
        msg = (
            f"This function depends on a script called get-pip.py from {INSTALL_PIP_URL}. "
             "To continue please download this file and put it in the 'embedded-python' folder.\n"
             "Note: Running code downloaded from the internet is dangerous, please inspect the downloaded file after downloading it"
        )
        raise RuntimeError(msg)

    os.system(f"{os.path.join(extracted_folder, 'python.exe')} {get_pip_path}")

    with open(os.path.join(extracted_folder, pth), "r") as f:
        lines = f.readlines()
        for i, line in enumerate(lines):
            if line == "#import site\n":
                lines[i] = "import site\n"

    with open(os.path.join(extracted_folder, pth), "w") as f:
        f.writelines(lines)


def make_zip(optimize=2):
    _, _, extracted_folder, pth = get_embedded_paths()
    site_package = os.path.join(extracted_folder, "Lib", "site-packages")

    lib_zip_file = os.path.join(extracted_folder, "lib.zip")
    if os.path.exists(lib_zip_file):
        os.remove(lib_zip_file)

    exclude = ["pip", "pkg_resources", "setuptools", "wheel", "__pycache__"]
    def include(file):
        if os.path.isfile(os.path.join(site_package, file)):
            return False

        if file in exclude:
            return False

        if file.endswith("dist-info"):
            return False

        return True
    list_dir = os.listdir(site_package)
    folders = list(filter(include, list_dir))

    if len(folders) == 0:
        raise RuntimeError("There are no packages to include in the zip file, install a package through pip first")

    zip = zipfile.PyZipFile(lib_zip_file, "w", optimize=optimize)
    for f in folders:
        zip.writepy(os.path.join(site_package, f))
        if f == "certifi":
            zip.write(os.path.join(os.path.join(site_package, f, "cacert.pem")), arcname="certifi/cacert.pem")

    zip.close()

    with open(os.path.join(extracted_folder, pth), "r") as f:
        lines = f.readlines()

    if "lib.zip\n" not in lines:
        lines.insert(1, "lib.zip\n")
        with open(os.path.join(extracted_folder, pth), "w") as f:
            f.writelines(lines)


def main():
    if 'all' in sys.argv:
        extract_python_zip(force='--force' in sys.argv)
        install_pip()
        return

    if 'extract' in sys.argv:
        extract_python_zip(force='--force' in sys.argv)
        return

    if 'pip' in sys.argv:
        install_pip()
        return

    if 'make_zip' in sys.argv:
        make_zip()
        return


if __name__ == "__main__":
    main()
