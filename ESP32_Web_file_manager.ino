//IDE 2.3.3 - - esp32 3.1.1
#include <WiFi.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <time.h>


//=======Defines

#define WIFI_SSID "Test"
#define WIFI_PASSWORD "Password1"


//=======SETTINGS

WebServer server(80);

/*
    Webserver
*/
void serverSetup() {

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
  } else {
    Serial.println("LittleFS initialized.");
  }
  Serial.println();

  server.serveStatic("/main", LittleFS, "/index.html");
  server.serveStatic("/style.css", LittleFS, "/style.css", "text/css");
  server.on("/", HTTP_GET, handleRoot);
  server.on("/reset", handleReset);
  server.onNotFound(handleNotFound);
  server.begin();  //Start the server
  Serial.println("Server is now listening ...");
  Serial.println();
}

void handleRoot() {
  if (LittleFS.exists("/index.html")) {
    server.sendHeader("Location", "/main");
    server.send(302, "text/plain", "Umleitung auf /main");
  } else {
    String redirectPage = "<html><body style='text-align:center;'>";
    redirectPage += "<p>index.html wurde nicht gefunden.</p>";
    redirectPage += "<p><a href='/fs'>File-Server</a></p>";
    redirectPage += "</body></html>";

    server.send(200, "text/html", redirectPage);
  }
}

void handleReset() {
  server.send(200, "text/plain", "RESET ESP in 2 Seconds...");
  delay(2000);
  ESP.restart();
}

void handleNotFound() {
  String msg = "File Not Found\n\n";
  msg += "URI: ";
  msg += server.uri();
  msg += "\nMethod: ";
  msg += (server.method() == HTTP_GET) ? "GET" : "POST";
  msg += "\nArguments: ";
  msg += server.args();
  msg += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    msg += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/html", msg);
}

/*
    File management
*/
void fileserverSetup() {
  server.on("/fs", HTTP_GET, handleFileList);
  server.on("/upload", HTTP_POST, sendResponce, handleFileUpload);
  server.on("/delete", HTTP_GET, handleFileDelete);
  server.on("/view", HTTP_GET, handleFileView);
  server.on("/format", HTTP_GET, handleFormat);
}

void handleFileList() {
  String files;
  File root = LittleFS.open("/");
  File file = root.openNextFile();

  while (file) {
    char buffer1[20];
    size_t sizeofFile = file.size();
    formatBytes(sizeofFile, buffer1, sizeof(buffer1));
    //formatBytes(file.size(), sizeBuffer, sizeof(sizeBuffer));
    files += "<div class='file-container'>";
    files += "<span class='filename'>" + String(file.name()) + "</span>";
    files += "<span class='file-size'>" + String(buffer1) + "</span>";
    files += "<span class='file-date'>" + getLastModified(file) + "</span>";
    files += "<span class='file-actions'>";
    files += "<a href='/delete?file=/" + String(file.name()) + "'>Delete</a>";
    files += "<a href='/view?file=" + String(file.name()) + "'>View</a>";
    files += "</span></div>";
    file = root.openNextFile();
  }

  String html = "<html><head>";
  html += "<style>body { text-align: center; }";
  html += ".container { display: inline-block; text-align: center; padding: 20px; border: 1px solid #ccc; border-radius: 10px; }";
  html += ".file-container { margin: 10px 0; display: flex; align-items: center; }";
  html += ".filename { flex: 1; margin-right: 10px; text-align: left; }";
  html += ".file-size { font-size: 60%; margin-right: 10px; }";
  html += ".file-date { font-size: 60%; margin-right: 10px; }";
  html += ".file-actions { display: flex; }";
  html += ".file-actions a { margin-right: 5px; text-decoration: none; color: #007BFF; }";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h2 style='margin-top: 0;'>Files on LittleFS</h2>";
  html += "<div class='file-list'>" + files + "</div>";
  html += "<br>";
  html += "<hr>";
  html += "<h3>Upload</h3>";
  html += "<form method='post' action='/upload' enctype='multipart/form-data'>";
  html += "<input type='file' name='upload[]' id='uploadFile' multiple>";
  html += "<input type='submit' value='Upload'>";
  html += "</form>";
  html += "<br>";
  html += "<hr>";
  html += "<h3>Format Filesystem</h3>";
  html += "<form method='get' action='/format'>";
  html += "<input type='submit' value='Format'>";
  html += "</form>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

void handleFileUpload() {
  if (server.uri() != "/upload") return;
  HTTPUpload &upload = server.upload();
  static File file;


  if (upload.status == UPLOAD_FILE_START) {
    String filename = "/" + upload.filename;
    Serial.println("Upload started: " + filename);

    file = LittleFS.open(filename, "w");
    if (!file) {
      Serial.println(F("Failed to open file for writing"));
      return;
    }
    Serial.println(F("File opened for writing"));
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (file) {
      size_t bytesWritten = file.write(upload.buf, upload.currentSize);
      if (bytesWritten != upload.currentSize) {
        Serial.println(F("Error writing to file"));
      } else {
        Serial.println("Bytes written to file: " + String(upload.currentSize));
      }
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (file) {
      file.close();
      Serial.println(F("Upload finished. File closed"));
    } else {
      Serial.println(F("Upload finished, but file was not open"));
    }
  }
}

void handleFileDelete() {
  String filename = server.arg("file");
  Serial.println("Delete: " + filename);

  if (LittleFS.remove(filename)) {
    Serial.println(F("File deleted"));
  } else {
    Serial.println(F("Failed to delete file"));
  }
  sendResponce();
}

void handleFileView() {
  String filename = server.arg("file");
  Serial.println("View: " + filename);

  if (!filename.startsWith("/")) {
    filename = "/" + filename;
  }

  File file = LittleFS.open(filename, "r");
  if (!file) {
    Serial.println(F("Failed to open file for reading"));
    server.send(404, "text/plain", "File not found");
    return;
  }

  String content = file.readString();
  file.close();

  String html = "<html><body><h2>File Viewer</h2><p>Content of " + filename + ":</p><pre>" + content + "</pre></body></html>";
  server.send(200, "text/html", html);
}

void handleFormat() {
  LittleFS.format();
  Serial.println(F("Formating LittleFS"));
  sendResponce();
}

void sendResponce() {
  server.sendHeader("Location", "/fs");
  server.send(303, "message/http");
}

String getLastModified(File file) {
  time_t lastWriteTime = file.getLastWrite();
  struct tm *timeinfo;
  char buffer[20];
  timeinfo = localtime(&lastWriteTime);
  strftime(buffer, sizeof(buffer), "%d:%m:%y %H:%M", timeinfo);
  return String(buffer);
}
/*
    File management
*/
/*
    End of Webserver
*/

// Readable display of memory sizes
void formatBytes(size_t bytes, char *buffer, size_t bufferSize) {
  if (bytes < 1024) {
    snprintf_P(buffer, bufferSize, PSTR("%zu%s"), bytes, PSTR(" Byte"));
  } else if (bytes < 1048576) {
    dtostrf(static_cast<float>(bytes) / 1024.0, 6, 2, buffer);
    strcat_P(buffer, PSTR(" KB"));
  } else {
    dtostrf(static_cast<float>(bytes) / 1048576.0, 6, 2, buffer);
    strcat_P(buffer, PSTR(" MB"));
  }
}
// Readable display of memory sizes



//============SETUP

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP-Adresse: " + WiFi.localIP().toString());
  serverSetup();
  fileserverSetup();
  configTime(0, 0, "pool.ntp.org");
}

//============LOOP

void loop() {
  server.handleClient();
}
