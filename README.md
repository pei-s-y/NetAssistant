# NetAssistant - Qt Upper Computer System

A professional desktop application developed for the **UESTC (电子科技大学攀登计划)**. This project serves as the high-performance upper computer system for data acquisition and system monitoring.

## 🚀 Key Features

* **UDP Network Communication**: Implemented high-speed data transmission based on the UDP protocol for low-latency hardware interaction.
* **Real-time Data Processing**: Built-in backend logic to filter and process incoming data streams efficiently.
* **Dynamic Waveform Plotting**: Real-time visualization of system parameters with responsive UI feedback.
* **Robust Framework**: Developed using **Qt/C++** to ensure cross-platform stability and high performance.
* **TCP Network Communication**: developing....

## 🛠️ Technical Stack

* **Framework**: Qt 6.5.3 (C++)
* **Network**: UDP Socket Programming
* **Tools**: MinGW 64-bit, Git
* **Hardware Integration**: Designed to interface with specialized sensing systems (e.g., QCM modules).

## 📂 Project Structure

* `NetAssistant.pro`: Main project configuration file.
* `mainwindow.cpp`: Core logic for UI events, UDP handling, and plotting.
* `icon.rc`: Resource configuration for application branding.
* `*.ui`: User interface design files.

## 🏗️ Getting Started

1.  Clone the repository:
    ```bash
    git clone https://github.com/pei-s-y/NetAssistant.git
    ```
2.  Open `NetAssistant.pro` in **Qt Creator**.
3.  Configure the project using the **Desktop Qt 6.5.3 MinGW 64-bit** kit.
4.  Build and Run.

## 📄 License
This project is licensed under the **MIT License**.

---
**Developed by Frieren @ UESTC**