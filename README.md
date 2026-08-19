# Anoma-Mali
-Anoma-Mali comes from Anomaly and Mali together.
-Anoma-Mali Is a driver im creating for Emulation.
-Anoma-Mali is a custom driver designed to improve compatibility and research for emulation on ARM Mali GPUs. 
## 🎯 Goals
-Research Mali GPU architecture to make It run    smooth on PC emulation environments 
-Research Mali GPU architecture and registers
- Build driver components optimized for software emulation environments
- Prove that writing a Mali driver is possible step-by-step.
## ⚠️ WARNING! 
Do NOT download,copy links or enter links from YouTube,Telegram or other platforms outside of github,if you want the official DEV channel it Is on Github only. 
## 🌟 Key Features
- **Based on Mesa 25.0.7:** Packed with the latest CPU overhead reductions and Zink optimizations.
- **Mali Optimized:** Better shader compilation (NIR) and improved stability over stock drivers.
- **Universal Zink Backend:** Translates OpenGL calls straight into Vulkan for your Mali GPU.

## 🛠️ Manual Installation Guide
To use these raw `.so` libraries in Winlator (tested on glibc builds):

1. Download the full driver package from the **Releases** section.
2. Open Winlator, run your container, and open the internal **File Manager**.
3. Copy all the `.so` files into the following Linux system directory:
   `Z:\usr\lib\dri\` *(Create the `dri` folder if it doesn't exist)*.
4. Make sure the files are named exactly: `zink_dri.so`, `panfrost_dri.so`, `panthor_dri.so`.
