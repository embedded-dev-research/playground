# SYCL Windows CPU example

This folder contains a minimal SYCL example for **Windows + CPU** and a full,
step-by-step instruction from zero to build and run it.

## 0) Prerequisites (from zero)

1) Install **Visual Studio 2022 Community** with **Desktop development with C++**.
2) Install **Git**, **Python 3**, **CMake** (and optionally **Ninja**).
3) Build the Intel SYCL toolchain (DPC++):

Open **"x64 Native Tools Command Prompt for VS 2022"** and run:

```bat
set DPCPP_HOME=%USERPROFILE%\sycl_workspace
mkdir %DPCPP_HOME%
cd %DPCPP_HOME%

git clone --config core.autocrlf=false https://github.com/intel/llvm -b sycl
python %DPCPP_HOME%\llvm\buildbot\configure.py
python %DPCPP_HOME%\llvm\buildbot\compile.py
```

This creates the toolchain in:

```
%DPCPP_HOME%\llvm\build\install\bin
```

4) Install **Intel CPU OpenCL Runtime** (needed so SYCL has a CPU device).

### 4.1 Download installer (no GUI required)

```bat
powershell -NoProfile -Command "Invoke-WebRequest -Uri https://registrationcenter-download.intel.com/akdlm/IRC_NAS/ad824c04-01c8-4ae5-b5e8-164a04f67609/w_opencl_runtime_p_2025.3.1.762.exe -OutFile C:\Windows\Temp\w_opencl_runtime_p_2025.3.1.762.exe"
```

### 4.2 Extract MSI silently

```bat
mkdir C:\Windows\Temp\opencl_cpu_extract
"C:\Windows\Temp\w_opencl_runtime_p_2025.3.1.762.exe" -x -s -f C:\Windows\Temp\opencl_cpu_extract -l C:\Windows\Temp\opencl_cpu_extract.log
```

### 4.3 Install MSI silently

```bat
msiexec /i C:\Windows\Temp\opencl_cpu_extract\w_opencl_runtime_p_2025.3.1.762.msi /qn /norestart /l*v C:\Windows\Temp\opencl_cpu_install.log ACCEPTEULA=YES ACCEPT_EULA=YES
```

### 4.4 Verify runtime installed

```bat
reg query "HKLM\SOFTWARE\Khronos\OpenCL\Vendors"
```

You should see `intelocl64.dll` in the output.

## 1) Clone this repo

```bat
git clone https://github.com/allnes/playground
cd playground\sycl-win-cpu
```

## 2) Configure environment

Open **"x64 Native Tools Command Prompt for VS 2022"** and set:

```bat
set DPCPP_HOME=%USERPROFILE%\sycl_workspace
set PATH=%DPCPP_HOME%\llvm\build\install\bin;%PATH%
```

Optional: force CPU device explicitly:

```bat
set SYCL_DEVICE_FILTER=opencl:cpu
```

## 3) Verify SYCL device list

```bat
sycl-ls
```

Expected output includes a CPU OpenCL device, for example:

```
[opencl:cpu][opencl:0] Intel(R) OpenCL, Intel(R) Xeon(R) ...
```

If you see `No platforms found`, the OpenCL CPU runtime is missing or not
registered. Re-run step 4 or reboot.

## 4) Configure and build

If you have Ninja:

```bat
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=%DPCPP_HOME%\llvm\build\install\bin\clang++.exe
cmake --build build
```

Without Ninja, use the default generator:

```bat
cmake -S . -B build -DCMAKE_CXX_COMPILER=%DPCPP_HOME%\llvm\build\install\bin\clang++.exe
cmake --build build
```

## 5) Run

```bat
build\sycl_hello.exe
```

Expected output:
- `Device: Intel(R) Xeon(R) ...`
- `Result: 42`

## Troubleshooting (important)

- `SYCL exception: No device of requested type available.`
  - Install the CPU OpenCL runtime (step 4).
  - Ensure `SYCL_DEVICE_FILTER=opencl:cpu` is set.
- If `sycl-ls` shows nothing:
  - Check registry key: `HKLM\SOFTWARE\Khronos\OpenCL\Vendors`.
  - Reboot after installing runtime.



Note: The sample uses `sycl::cpu_selector_v` to force CPU. If you want to use the default device selection, change it to `sycl::queue q;`.
