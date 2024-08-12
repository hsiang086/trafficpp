# Trafficpp

Trafficpp is a simple and lightweight network traffic sniffer built using C++ and `libpcap`. It captures and analyzes network packets, providing detailed information about Ethernet, IP, and TCP headers.

## Features

- Network Traffic Sniffing: 
    - Capture and analyze network packets.
- Packet Information:
    - Ethernet, IP, and TCP headers.
- Packet Filtering:
    - Filter packets based on IP address, port number, and protocol.
- Packet Logging:
    - Log packet information to a file.

## Installation
### Prerequisites
- CMake
    - Install CMake on MacOS:
        ```bash
        brew install cmake
        ```
    - Install CMake on Ubuntu:
        ```bash
        sudo apt-get install cmake
        ```
    - Install CMake on Fedora:
        ```bash
        sudo dnf install cmake
        ```
- `libpcap` library
    - Install `libpcap` on MacOS:
        ```bash
        brew install libpcap
        ```
    - Install `libpcap` on Ubuntu:
        ```bash
        sudo apt-get install libpcap-dev
        ```
    - Install `libpcap` on Fedora:
        ```bash
        sudo dnf install libpcap-devel
        ```

### Build
```bash
git clone https://github.com/hsiang086/traffcpp.git
cd traffcpp
mkdir build && cd build && cmake ..
make
```

## Usage
```bash
sudo ./traffcpp [options]
```

- `--help` or `-h`: Display help message.
- `--run` or `-r`: Start capturing network packets.
- `--version` or `-v`: Display version information.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
