#include <Arduino.h>
#include <WiFi.h>
#include <SD.h>
#include <ArduinoJson.h>
// include esp32-cam library
#include "esp_camera.h"
#include <mbedtls/sha256.h>
#include <mbedtls/base64.h>
#include <WiFiClientSecure.h>

JsonDocument LoadConfig();
void ConnectToWifi(String ssid, String password);

// Returns a token for use with Google Cloud PubSub
String AuthenticateWithGCSPubSub(
    String sa_email,
    String sa_private_key,
    String project_id,
    String api_host);

String GenerateJWT(String privateKey, String clientEmail);
String getAccessToken(const String &jwt);
String base64UrlEncode(const uint8_t *input, size_t length);
String base64EncodeMessage(const String &message);

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
  config["ssid"] = "scoltock";
  config["wifi_password"] = "nowireshere";
  config["gcs_sa_email"] = "stormwater@home-automation-453101.iam.gserviceaccount.com";
  config["gcs_sa_private_key"] = "-----BEGIN PRIVATE KEY-----\nMIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDVtQHHCnybKopa\nKrzSnFkTwYYLMxbfZPieJeKoaE1P31X3EMpC/cNcMV1ISMwjqZIJUHHalcfBiHl4\nKWwbKEj/Eo11gOSSOOjjaZJt3+CCAc93DUVWI+0rXqDEnIbTZrc/goRtqljhPb58\nYTLQk3R8iFEASnqf5cceKW2euctrb4aSJHbTrHuwTRpmXaZ4ZDVFTRmF4Pr5o0qe\nxdJGQJphcjS1gBXs2Vf9K7Um/LwkAqP5QY6As6oVB4hkryel1BW49KR6DVUS5E+b\nhOzMoGKni9usu+JHvkpnKtlbyCC/ZV9lJZ7ip0FhD7FtWqGaYXanB5Tv8+tdKJQe\niwkSJ3a9AgMBAAECggEAEwgyNtQvwa/Zr2sD8uu6oSBIfEHaBR4caBUuqNnVHMpi\ngQTNRocL+W9qA9B74act2sd/xDw3lHw/eRyTRJ3jVlgEIhVPBdisoOgbgs5/04Qu\nwkb7yE6dxhGA1tGrzjLlGfJZMd0MWk7h4njTijC/nIGVWSXI4umXQI/m885PF+um\nkcbOLWo24V9cCVF6DJTxI0LKgNGtFeaj/qFTgT3QF1zEKPSBeqG5RqlwtXNvmM3k\nJy4wl4Iwn/EblXuGDCMQR9nGoN6j3Y+LHunsi8Gjqs1/yadC/niOv+Nqf5xoX5V3\nzTdk2pJTAHlbYPUDKVv6FQmDkwagIMAqF048wmKscQKBgQDs4wgxJaKlME2o9fd4\nAQb/ZCaHpn9kO3oDGLltveSguCLs0SJslN6G/aTiUOqg4jGSN64pnZEKdx8MJOS6\nz7GJf2OiYkZ5C921f89gQM9/AhmQiI790v46oJzgJmv9wGFjXkdHOws2jNLNegz9\nbF0/ABO4dpxSlXj9TEU9KMtSOQKBgQDm8zCJ1OIFL+ranLL3s72LI10DRdAUxtBl\n154S9+4WKfaA2+Sr3BU5rfpW7l0m743f8qRmi5ixo2cR1hQteIz7dmDkgGtlqEQh\n3NplEAHzvwK6D7Ysd5O6wG23zyTT2QQknOlZQdeU5KuatYztySDGKKuBkYXzu61F\nexUsOhc4pQKBgGxmgAstc74v2nnlBLePkMox5EfS8xzE8tKT79a+PO8nFCRWl3Ak\nt6gCTf+ak6PHnnOQs0wr5IFrfXOrNlgeTtnix485dZJS0cQKlrUvM0Sli1lOhFC2\nysu9T8xrCKP38xjrvaZk6H6v9o88uNEHU6xOtreE34gTCmivDEgucMxRAoGAPaQj\nDGa5fNDxSjAQxHRA3uYtaTY095apAMWv9zgdX+ULWhFW01gGgkKhUpqEWmQN5fwJ\nTSVtN9x5IhWVhR1r46IQ8mwkPhnPNYqQ7/B51OjifW68HNo9n9bEcg7jsXd2157Z\n2WztcVwnQT+7wauxB7LLM+X3brpk4OqdGBTZJdECgYB6y+iKrTZAcoC2b4hMkx/N\nUSw3osK834QVe7JuovEDZ+LQWtTJYAEHDMqFFLKavWr2aRVUIo4dd4Df5gmfQrfq\noRw9NlqwhJwSmcIGimx4ZDHGFTCTyjWnzXzRywfbTMm6oQEmWtv6ZRUh3/1Q02JB\nhEsDMnGoohm1Q67m/lLtxg==\n-----END PRIVATE KEY-----\n";
  config["gcs_project_id"] = "home-automation-453101";
  config["gcs_api_host"] = "pubsub.googleapis.com";
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

String AuthenticateWithGCSPubSub(
    String sa_email,
    String sa_private_key,
    String project_id,
    String api_host)
{

  String jwt = GenerateJWT(sa_private_key, sa_email);
  String accessToken = getAccessToken(jwt);

  return accessToken;
}

String base64UrlEncode(const uint8_t *input, size_t length)
{
  size_t olen;
  unsigned char *output = (unsigned char *)malloc(length * 2); // Allocate enough memory

  if (output == NULL)
  {
    Serial.println("Memory allocation failed");
    return "";
  }

  mbedtls_base64_encode(output, length * 2, &olen, input, length);

  String encoded = String((char *)output, olen);
  free(output);

  // Base64URL encoding. remove padding and replace unsafe characters.
  String base64Url = "";
  for (int i = 0; i < encoded.length(); i++)
  {
    if (encoded[i] == '+')
    {
      base64Url += '-';
    }
    else if (encoded[i] == '/')
    {
      base64Url += '_';
    }
    else if (encoded[i] == '=')
    {
      // remove padding
    }
    else
    {
      base64Url += encoded[i];
    }
  }

  return base64Url;
}

String base64EncodeMessage(const String &message)
{
  size_t olen;
  unsigned char *output = (unsigned char *)malloc(message.length() * 2);
  if (output == NULL)
  {
    Serial.println("Memory allocation failed");
    return "";
  }
  mbedtls_base64_encode(output, message.length() * 2, &olen, (const unsigned char *)message.c_str(), message.length());
  String encoded = String((char *)output, olen);
  free(output);
  return encoded;
}

String GenerateJWT(String privateKey, String clientEmail)
{
  // Header
  JsonDocument headerDoc;
  headerDoc["alg"] = "RS256";
  headerDoc["typ"] = "JWT";
  String header;
  serializeJson(headerDoc, header);
  String encodedHeader = base64UrlEncode((const uint8_t *)header.c_str(), header.length());

  // Payload
  JsonDocument payloadDoc;
  payloadDoc["iss"] = clientEmail;
  payloadDoc["scope"] = "https://www.googleapis.com/auth/pubsub"; // Adjust scope
  payloadDoc["aud"] = "https://oauth2.googleapis.com/token";
  ;

  payloadDoc["exp"] = time(nullptr) + 3600; // Expires in 1 hour
  payloadDoc["iat"] = time(nullptr);
  String payload;
  serializeJson(payloadDoc, payload);
  String encodedPayload = base64UrlEncode((const uint8_t *)payload.c_str(), payload.length());

  String unsignedToken = encodedHeader + "." + encodedPayload;

  // Sign
  mbedtls_pk_context pk;
  mbedtls_pk_init(&pk);
  mbedtls_pk_parse_key(&pk, (const unsigned char *)privateKey.c_str(), strlen(privateKey.c_str()) + 1, NULL, 0);

  uint8_t hash[32];
  mbedtls_sha256((const unsigned char *)unsignedToken.c_str(), unsignedToken.length(), hash, 0);

  size_t sig_len;
  uint8_t signature[256];
  mbedtls_pk_sign(&pk, MBEDTLS_MD_SHA256, hash, sizeof(hash), signature, &sig_len, NULL, NULL);
  mbedtls_pk_free(&pk);

  String encodedSignature = base64UrlEncode(signature, sig_len);

  return unsignedToken + "." + encodedSignature;
}

String getAccessToken(const String &jwt)
{
  Serial.println("\n\n\n\nRequesting access token...");
  WiFiClientSecure client;
  // client.setCACert(caCert); // Use your CA certificate
  client.setInsecure(); // Remove for production!
  if (client.connect("oauth2.googleapis.com", 443))
  {
    String request = "POST /token HTTP/1.1\r\n"
                     "Host: oauth2.googleapis.com\r\n"
                     "Content-Type: application/x-www-form-urlencoded\r\n"
                     "Content-Length: ";

    String body = "grant_type=urn%3Aietf%3Aparams%3Aoauth%3Agrant-type%3Ajwt-bearer&assertion=" + jwt;
    request += String(body.length()) + "\r\n\r\n" + body;

    client.print(request);

    String response = "";
    String responsebody = "";
    bool headersFinished = false;

    while (client.connected())
    { // This is a rough method of parsing the response. It assumes the headers are finished when it encounters a blank line.
      if (!headersFinished)
      {
        String line = client.readStringUntil('\n');
        if (line == "\r")
        {
          headersFinished = true;
        }
        response += line + "\n";
      }
      else
      {
        String chunkSizeStr = client.readStringUntil('\n'); // discard chuck size
        responsebody = client.readString();
        responsebody = responsebody.substring(0, responsebody.length() - 4); // remove the traling '0'
        break;
      }
    }

    client.stop();

    // Parse the JSON response to extract the access token
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, responsebody);
    if (error)
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return "";
    }
    String accessToken = doc["access_token"].as<String>();

    return accessToken;
  }
  else
  {
    Serial.println("Connection to oauth2.googleapis.com failed");
    return "";
  }
}