#include <Arduino.h>
#include <WiFi.h>
#include <SD.h>
#include <ArduinoJson.h>
// include esp32-cam library
#include "esp_camera.h"
#include <mbedtls/sha256.h>
#include <mbedtls/base64.h>
#include <WiFiClientSecure.h>
#include "../secrets/secrets.h"
#include "gcs.h"

JsonDocument LoadConfig();
void ConnectToWifi(String ssid, String password);

// Returns a token for use with Google Cloud PubSub
String AuthenticateWithGCSPubSub(
    String sa_email,
    String sa_private_key,
    String project_id,
    String api_host);

void setup()
{

  Serial.begin(115000);
  Serial.println("\n************\nStarting...\n*************\n");

  JsonDocument configDoc = LoadConfig();
  ConnectToWifi(configDoc["ssid"], configDoc["wifi_password"]);
  String GenerateJWT(String privateKey, String clientEmail);

  String gcs_token = AuthenticateWithGCSPubSub(
      configDoc["gcs_sa_email"],
      configDoc["gcs_sa_private_key"],
      configDoc["gcs_project_id"],
      configDoc["gcs_api_host"]);

  Serial.println("GCS Token: " + gcs_token);
}

void loop()
{
  // flash the esp32 onboard led
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
}

JsonDocument LoadConfig()
{
  JsonDocument config;
  config["ssid"] = SSID;
  config["wifi_password"] = WIFI_PASSWORD;
  config["gcs_sa_email"] = GCS_SA_EMAIL;
  config["gcs_sa_private_key"] = GCS_SA_PRIVATE_KEY;
  config["gcs_project_id"] = GCS_PROJECT_ID;
  config["gcs_api_host"] = GCS_PUBSUB_API_HOST;
  return config;
}

void ConnectToWifi(String ssid, String password)
{
  Serial.print("Connecting to WiFi: " + ssid);
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to WiFi: ");
  Serial.println(WiFi.localIP());

  Serial.print("Updating time from NTP server");
  configTime(0, 0, "pool.ntp.org");
  while (time(nullptr) < 1000)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Time updated");
}