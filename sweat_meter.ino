# include <ESP8266WiFi.h>
# include "sweat_meter.h"

const char* SSID = "ustcszmbit";
const char* PASSWORD = "ustcmbit";
unsigned int DATA_ID = 0;

WiFiServer server(80);                                // 监听 80 端口

void setup()
{
    Serial.begin(115200);
    
    Serial.println();
    // Serial.printf("Connecting to %s", SSID);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.printf("Connected, IP address: %s\n", WiFi.localIP().toString().c_str());

    server.begin();
    Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

void loop()
{
    WiFiClient client = server.available();

    if (client)
    {
        Serial.println("[Client connected]");

        while (client.connected())
        {
            if (client.available())
            {
                String line = client.readStringUntil('\r');
                Serial.print(line);

                if (line.length() == 1 && line[0] == '\n')
                {
                    client.println(prepareHtmlPage());
                    break;
                }
            }
        }
        
        client.stop();
        Serial.println("[Client disconected]");
    }
    
}

String prepareHtmlPage()
{
    String htmlPage;
    String data = String(DATA_ID++, DEC) + "-" + analogRead(A0);

    htmlPage.reserve(512);
    htmlPage = F("HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Connection: close\r\n"                               //  HTTP/1.0 的默认值, 在  HTTP/1.1 中默认值为 keep-alive
                 "\r\n"
                 "<!DOCTYPE HTML>"
                 "<html>"
    );
    htmlPage += F("Hello World!\r\n");
    htmlPage += data;
    htmlPage += F("</html>\r\n");

    return htmlPage;
}
