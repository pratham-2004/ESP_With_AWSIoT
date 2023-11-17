/* ESP32 AWS IoT
 *  
 * Simplest possible example (that I could come up with) of using an ESP32 with AWS IoT.
 *  
 * Author: Anthony Elder 
 * License: Apache License v2
 */
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // install with Library Manager, I used v2.6.0

const char* ssid = "ARYAMAN";
const char* password = "Aryaman@6060";

const char* awsEndpoint = "a2itom115cy0mv-ats.iot.eu-north-1.amazonaws.com";

// Update the two certificate strings below. Paste in the text of your AWS 
// device certificate and private key. Add a quote character at the start
// of each line and a backslash, n, quote, space, backslash at the end 
// of each line:

// xxxxxxxxxx-certificate.pem.crt
const char* certificate_pem_crt = \

//-----BEGIN CERTIFICATE-----
R"EOF(MIIDWjCCAkKgAwIBAgIVAIO3p5YY4kVZJeZxc0JY4DN7+cUgMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMzExMTcxMTE4
NDdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC+GbpzI6JmeHXt5z68
gqaJGwv5afBjFh4z9k2flerT3qJT33bK7oaUARygjwFRsTN/Z3cg/IySoEGDaryl
mrlXJdnC5e5LSEhjwAKxQ4aCJ6Fh01q4l/CPIRY/usenvgHPtHwN95hi3UGJLtaE
7myOcdUURLCuDZrXUredT6nT8dv1HcP5aPusWiuR9KVvDmklWHBH2TJopMAv1X1I
AKZ2ptSe5eGGDgnlp23E4NC0q/hcW4tR0pAcGvzNYUu+GUO58Vq32idnN5U32SiZ
kvyQsHsxYDY9NIe8utl8CDhJK92EdgYkJ1fE+jFNmUqTfPZmW7zb5FPhGSqoMbEh
XnFvAgMBAAGjYDBeMB8GA1UdIwQYMBaAFECLJBMRRrcRwjJqebK42nTae1noMB0G
A1UdDgQWBBQCIFqBPJmjUgnRA1rLHN4K9Vj9qTAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAkJVe02GwCzFl3HhpVATyPSVO
QiEgsTAzUKjcYJUb+tuBdhp2v2Vw2erzYa0Xrr4Il/6TjApnMaTsGNtUvx35D7xa
pR32c8NjtROa14TfpD33sU2DRf1S127/Es+FRF9+stQ8x4/OoVED6+LmTJ9gGnID
IEBwLXhJR7bm0lO+uIG9WVOcSOJmBAS5n4C8dZLFom79I2OXtBASGFbfIoL71dQ1
md75NPCkueIjg2QOYY7I0K1/jxxsa0eTFVd92uWlVIdY6MfbbLmtbHm42AYVCW30
DRAA0uoGS8c/cP0/C3plARGW4BPc9LKuI7qRDg8jIguM35fBLOSmALhIllUQ0w==)EOF";
//-----END CERTIFICATE-----


// xxxxxxxxxx-private.pem.key
const char* private_pem_key = \

//-----BEGIN RSA PRIVATE KEY-----
R"EOF(MIIEowIBAAKCAQEAvhm6cyOiZnh17ec+vIKmiRsL+WnwYxYeM/ZNn5Xq096iU992
yu6GlAEcoI8BUbEzf2d3IPyMkqBBg2q8pZq5VyXZwuXuS0hIY8ACsUOGgiehYdNa
uJfwjyEWP7rHp74Bz7R8DfeYYt1BiS7WhO5sjnHVFESwrg2a11K3nU+p0/Hb9R3D
+Wj7rForkfSlbw5pJVhwR9kyaKTAL9V9SACmdqbUnuXhhg4J5adtxODQtKv4XFuL
UdKQHBr8zWFLvhlDufFat9onZzeVN9komZL8kLB7MWA2PTSHvLrZfAg4SSvdhHYG
JCdXxPoxTZlKk3z2Zlu82+RT4RkqqDGxIV5xbwIDAQABAoIBAFY7FXhlEaWpvtTL
sImSOji3LBdXFtPblJe9gjiBqKOyua+SDjC/Qgyg9HtgYxjLaLDCHie4Z4ubZhxW
rnvwU2JUqRLfedhsUWnuD0qZdspTb8EQPna9dUF3oMAH2ckdR3UQSbWMOnOeKbww
ot5pyJRUznMDNaG+Sh6iRuf02vRoWSiILhW1TD1zyiswHBMv/6pjZYOwgSqI2N6F
VOUpAFyfyTDVhp47F0mxK87CY2a8o70JgDLUouNgbnpBrlWj7JtYFRO9K9zxwm4Y
X5ywvJNUitWQ9/jC8eddCZgG95jp+/mD70KwSaEbniZrdMw4a9QIFn2XF+q0SO0Z
wcinwQECgYEA5qEFiIsAdRXQUGOTaEDry2DAohy+NGDetXblDVQIsf4fWhCx75EI
/QbHDF1oXfBSzau8l+Q3838Xas3JHshfJntk33gpgH9UCtdDzxVKMuPWKVUmR3Oc
5lLqX1dSH4NUIPIB6hP8KRGLbn/PJtFloxLamgvoyD+WpTUc5KyIWCMCgYEA0wNX
sWQu6nTrVzG8Y3STb4V2UzSPg7jgMPZn3YmeWUd6M5eVSxHweSBjXUV0KI+3C1R3
2C3E3Jz8QHoU7sHDds8uGMUulrK5qPdcvaQb7b09Q3GeZb9wrxpSrGPy3LNdwccW
V4c3eg5S722I3zbRH/a+OIh+h3tgdS3kK8f5kEUCgYEAzmAmyHHCXgPAHj+Mx8zY
c2zH1Akud0Z75BxZiVy6TzScr4ajA0JgrbMj6T+oxe7o2dyk7bKuErmWpd8x6tbY
tmHS+VnYoYDPHshUiHl/pRxWJzRoBaZCL6Tgz6Ie/c4SOUz2Q5XQnptxjtbV8uGM
NqoHbbPcgVzNnFZz3Xk6pMsCgYB3iwU7Mg6CNQq+syaPEZQVMf7+1GAZt7sLAg2F
rFlRKvVIFTA2/0rE1Fx2kspj0Eda29epWQSWFzmxLXh+ardZr7Xlwo/KQCZ+WuTn
avmHjhYPB2c9LV+hU8BSea0060jKZwd5PtlnBIZnrh5ElsGS4i6RyIImLvdOa1lA
lGIGIQKBgB2dI5vjlgjSLRN425sjVnd7GoaWNeX8W/YQJO+Ljy8mZxmKlAAN3D6y
qpwla0MmFM/apjEkbmxs/sAQlKBK3A1qhFNdKMq0y3xS4ThXJMjvyLJJObdjAqrz
s0mjFOQ0FOlCav3rR3OUKb86twX0a1s+RvdFPr8iz9oBSczxSfnt)EOF";
//-----END RSA PRIVATE KEY-----


/* root CA can be downloaded in:
  https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem
*/
const char* rootCA = \
//-----BEGIN CERTIFICATE-----
R"EOF(MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5)EOF";
//-----END CERTIFICATE-----

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

void setup() {
  Serial.begin(115200); delay(50); Serial.println();
  Serial.println("ESP32 AWS IoT Example");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

  if (millis() - lastPublish > 10000) {
    String msg = String("Hello from ESP32") + ++msgCount;
    boolean rc = pubSubClient.publish("outTopic", msg.c_str());
    Serial.print("Published, rc="); Serial.print( (rc ? "OK: " : "FAILED: ") );
    Serial.println(msg);
    lastPublish = millis();
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if (!pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while (!pubSubClient.connected()) {
      Serial.print(".");
      if (pubSubClient.connect("ESPthingXXXX")) {
        Serial.println(" connected");
        pubSubClient.subscribe("inTopic");
      } else {
        Serial.print(" failed, rc=");
        Serial.print(pubSubClient.state());
        Serial.println(" try again in 1 second");
        delay(1000);
      }
    }
  } else {
    Serial.println("PubSubClient already connected");
  }
  pubSubClient.loop();
}
