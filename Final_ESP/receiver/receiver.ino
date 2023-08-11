/************************* File Information ***************************
 * Project Name:  ESP-NOW RECEIVER EXAMPLE
 * Author:        Jordan De Sotle
 * Date:          07/21/23
 * Description:   Code for ESP32 to receive ESP-NOW protocol
 *                Can be configured to receive different types of
 *                data over the ESP-NOW protocol
 * Usage:         Modify the following struct and functions to
 *                match the type of data you want to decode
 *
 *    TYPE        NAME
 *  - struct      data
 *  - void        print_data
 ************************* File Information ***************************/

//Includes
#include <esp_now.h>
#include <WiFi.h>

#include <string.h>

/***************************** Constants ******************************/

const bool DEBUG_MODE = true;

/***************************** Constants ******************************/

//Must match the sender structure
typedef struct data {
  int x;
  int y;
};

/***************************** Variables ******************************/

data received_data;

/***************************** Variables ******************************/

/***************************** Functions ******************************/
/*--------- DO NOT MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING ---------*/

// Callback for when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char mac_str[18];
  Serial.print("Packet from: ");
  // Copies the sender mac address to a string
  snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(mac_str);

  memcpy(&received_data, incomingData, sizeof(received_data));
  Serial.print("\tBytes received: ");
  Serial.println(len);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  data new_data = received_data;
  print_data(new_data);
}

/*--------- DO NOT MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING ---------*/

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
  Serial.println("--------------------------------------------------");
}

/***************************** Functions ******************************/
 
// Runs once after restart
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
