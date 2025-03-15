#include <Arduino.h>

// Generate an identity token for use with Google Cloud PubSub
String GenerateJWT(String privateKey, String clientEmail);
// Get an access token from Google Cloud OAuth server
String getAccessToken(const String &jwt);
String base64UrlEncode(const uint8_t *input, size_t length);
String base64EncodeMessage(const String &message);
//Return a JWT for use with GCS PubSub
String AuthenticateWithGCSPubSub(
    String sa_email,
    String sa_private_key,
    String project_id,
    String api_host);
