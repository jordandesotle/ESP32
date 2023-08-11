# ESP32

This repo consists of four files:
- sender.ino
- receiver.ino
- sniffer_final.ino
- spoofer_final.ino

## Usage

These scripts were written for ESP32 devices in order to replicate esp-now traffic.
By sending and receiving traffic between two known points using the sender.ino and receiver.ino files,
it is possible to sniff and spoof that data using a third ESP32 device.

I used the Arduino IDE to upload and program the devices.

### Step 1

Download the Arduino IDE from the following link:
https://www.arduino.cc/en/software

### Step 2

Next, you will need to download the ESP32 library in arduino. Here is a [link](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) on how to do that.

### Step 3

Now that you have the different ESP boards downloaded, select yours in **Tools > Board > *"Your ESP Board"***

You will want to plug your ESP device in now and sekect the port in **Tools > Port > *"Your device's port"***

### Step 4
Now you will want to change the MAC address in each of the programs to match your hardware. To determine your hardware address, copy and paste the [following script](https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/) into your IDE and upload it. Using the serial monitor, you will be able to see the MAC address of your ESP32 device.

### Step 5

Once you identify your hardware address, change the necessary variables to allow your ESP devices to comminucate properly. Here is a guide for each file:
- **sender.ino**: Change the *RECEIVER_MAC* variable to match the MAC address of the ESP that you want to send data to
- **receiver.ino**: No variables need changing. Since esp-now is peer-to-peer, the receiver can be set to accept incoming traffic to it's specified MAC address
- **sniffer_final.ino**: Change the *SPOOFED_MAC* variable to the MAC address of the intended receiver. By doing this, all traffic meant for the actual receiver will go to you as well
- **spoofer_final.ino**: Change *RECEIVER_MAC* and *SPOOFED_MAC* to the MAC address you want to send spoofed data to and the MAC address you want to send spoofed data from