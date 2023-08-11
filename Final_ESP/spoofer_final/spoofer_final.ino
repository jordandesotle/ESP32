/************************* File Information ***************************
 * Project Name:  ESP-NOW SPOOFER
 * Author:        Jordan De Sotle
 * Date:          07/21/23
 * Description:   Code for ESP32 to spoof ESP-NOW protocol
 *                Can be configured to spoof data using any
 *                mac address over the ESP-NOW protocol
 * Usage:         Modify the following struct and functions to
 *                match the type of data you want to transmit
 *
 *    TYPE        NAME
 *  - struct      data
 *  - void        send_data
 *  - data        package_data
 *  - void        print_data
 ************************* File Information ***************************/

// Includes
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

/***************************** Constants ******************************/

const uint8_t RECEIVER_MAC[6] = {0xB0, 0xA7, 0x32, 0x15, 0x19, 0x90};
const uint8_t SPOOFED_MAC[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
const bool DEBUG_MODE = true;

/***************************** Constants ******************************/

// Structure to package data
typedef struct data {
  int x;
  int y;
};

/***************************** Variables ******************************/

data test;
esp_now_peer_info_t peerInfo;

/***************************** Variables ******************************/

/***************************** Functions ******************************/
/*--------- DO NOT MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING ---------*/

// Function to change the MAC address of the ESP
bool change_mac(const uint8_t* new_mac) {

  // Initialize the Wi-Fi stack with default configuration
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_err_t ret = esp_wifi_init(&cfg);
  if (ret != ESP_OK) {
    Serial.println("Failed to initialize Wi-Fi");
    return 0;
  }
  if(DEBUG_MODE) { Serial.println("Successfully initialized Wi-Fi"); }

  // Print the MAC address
  uint8_t cur_mac[6];
  ret = esp_wifi_get_mac(WIFI_IF_STA, cur_mac);
  if (ret != ESP_OK) {
    Serial.println("Failed to get current Mac address");
    return 0;
  }
  if(DEBUG_MODE) { Serial.printf("Current MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", cur_mac[0], cur_mac[1], cur_mac[2], cur_mac[3], cur_mac[4], cur_mac[5]); }

  // Set the new base MAC address
  ret = esp_base_mac_addr_set(new_mac);
  if (ret != ESP_OK) {
    Serial.println("Failed to set new MAC address");
    return 0;
  }
  if(DEBUG_MODE) { Serial.println("Successfully set new Mac address"); }

  // Deinitialize Wi-Fi before reinitialization
  esp_wifi_deinit();
  if(DEBUG_MODE) { Serial.println("Deinitialized Wi-Fi"); }

  // Reinitialize Wi-Fi with the new MAC address
  ret = esp_wifi_init(&cfg);
  if (ret != ESP_OK) {
    Serial.println("Failed to reinitialize Wi-Fi");
    return 0;
  }
  if(DEBUG_MODE) { Serial.println("Successfully reinitialized Wi-Fi"); }

  // Get the MAC address of the Wi-Fi interface
  uint8_t macAddr[6];
  ret = esp_wifi_get_mac(WIFI_IF_STA, macAddr);
  if (ret != ESP_OK) {
    Serial.println("Failed to get MAC address");
    return 0;
  }
  if(DEBUG_MODE) { Serial.printf("New MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]); }

  // Start the Wi-Fi driver with the new MAC address
  ret = esp_wifi_start();
  if (ret != ESP_OK) {
    Serial.println("Failed to start Wi-Fi");
    return 0;
  }
  if(DEBUG_MODE) { Serial.println("Successfully started Wi-Fi"); }

  delay(100);
  return 1;

}

// Function to register peers
void register_peers() {

  esp_now_register_send_cb(OnDataSent);
   
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // register first peer  
  memcpy(peerInfo.peer_addr, RECEIVER_MAC, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

// Callback for when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char mac_str[18];
  Serial.print("Packet to:\t\t");
  // Copies the sender mac address to a string
  snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(mac_str);
  Serial.print("Packet from:\t\t");
  snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
           SPOOFED_MAC[0], SPOOFED_MAC[1], SPOOFED_MAC[2], SPOOFED_MAC[3], SPOOFED_MAC[4], SPOOFED_MAC[5]);
  Serial.println(mac_str);
  Serial.print("Send Status:\t\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

/*--------- DO NOT MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING ---------*/
/*------------- MODIFY THESE TO FIT YOUR DATA STRUCTURE --------------*/

// Function to send data over ESP-NOW
void send_data(data d) {

  // Send data
  esp_err_t result = esp_now_send(0, (uint8_t *) &d, sizeof(data));
  
  Serial.println("--------------------------------------------------");
  if (result == ESP_OK) {
    Serial.println("Sent with success.\tData:");
    print_data(d);
  }
  else {
    Serial.print("Error sending the data. Result: ");
    Serial.println(result);
  }

  delay(2000);

}

// Function to input data
data package_data() {

  //Assign values
  test.x = random(0,20);
  test.y = random(0,20);

  data d = test;
  return d;
}

// Function to print the fields of a struct
void print_data(data d) {

  // Name of vaiables in order of struct
  const char* fieldNames[] = {
    "x",
    "y"
  };

  // Value of variables in order of struct
  const void* fieldPointers[] = {
    &d.x,
    &d.y
  };

  int numFields = sizeof(fieldNames) / sizeof(fieldNames[0]);

  // Loops through each field in the struct
  Serial.println("----Data Packet----");
  for (int i = 0; i < numFields; i++) {
    const char* fieldName = fieldNames[i];
    const void* fieldValuePtr = fieldPointers[i];

    // Add a condition for each variable in the struct and change the datatype
    Serial.print(fieldName);
    Serial.print(": ");
    if (strcmp(fieldName, "x") == 0) {
      Serial.println(*static_cast<const int*>(fieldValuePtr));
    } else if (strcmp(fieldName, "y") == 0) {
      Serial.println(*static_cast<const int*>(fieldValuePtr));
    }
  }
  Serial.println("----Data Packet----");
}

/*------------- MODIFY THESE TO FIT YOUR DATA STRUCTURE --------------*/
/***************************** Functions ******************************/
 
// Runs once after restart
void setup() {
  Serial.begin(115200);

  // Function call to change_mac
  if(!change_mac(SPOOFED_MAC)) {
    Serial.println("Failed to update MAC Address");
  }
  if(DEBUG_MODE) { Serial.println("MAC Address updated successfully"); }

  WiFi.mode(WIFI_STA);
 
  // Initialize EPS-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  register_peers();
  
}

// Runs forever
void loop() {
  
  // Sends packaged data every 5 seconds
  send_data(package_data());
  delay(5000);
  
}

