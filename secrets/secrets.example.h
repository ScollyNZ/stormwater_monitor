  #include <Arduino.h>
// This file is used to store sensitive information that should not be shared
// with the public. This file should be added to the .gitignore file to prevent
// it from being uploaded to the repository. This file should be used to store
// sensitive information such as passwords, API keys, and other information that
// should not be shared with the public. This file should be included in the
// project as a header file that is included in the main source code file.

  const String SSID = "random_ssid";
  const String WIFI_PASSWORD = "hard_to_guess";
  const String GCS_SA_EMAIL = "something@project.iam.gserviceaccount.com";
  const String GCS_SA_PRIVATE_KEY = "-----BEGIN PRIVATE KEY-----\n<<KEY-MATERIAL>>\n-----END PRIVATE KEY-----\n";
  const String GCS_PROJECT_ID = "project_id_including_id_number";
  const String GCS_PUBSUB_API_HOST = "pubsub.googleapis.com";