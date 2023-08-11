/************************* File Information ***************************
 * Project Name:  ESP-NOW SENDER EXAMPLE
 * Author:        Jordan De Sotle
 * Date:          07/21/23
 * Description:   Code for ESP32 to send data over ESP-NOW protocol
 *                Can be configured to send different types of data
 *                over the ESP-NOW protocol
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

#include <string.h>

/***************************** Constants ******************************/

const uint8_t RECEIVER_MAC[6] = {0xB0, 0xA7, 0x32, 0x15, 0x19, 0x90};
const bool DEBUG_MODE = true;

/***************************** Constants ******************************/

//Structure to package data
typedef struct data {
  int x;
  int y;
};


data test;

esp_now_peer_info_t peerInfo;

/***************************** Variables ******************************/

/***************************** Functions ******************************/
/*--------- DO NOT MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING ---------*/

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
  Serial.println(WiFi.macAddress());
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
  // test.x = random(0,20);
  // test.y = random(0,20);

  test.x = 13;
  test.y = 10;


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