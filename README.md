# IOT47_wifi_ota
Thư viện upload firmware ota cho esp32/esp8266 arduino qua webserver
Phiên bản cho arduino ở đây: https://github.com/daonguyen207/espidf_webserver_ota
# Sử dụng
Thư viện này yêu cầu bạn cài ESPAsyncWebServer.h
# add thư viện
```
#include "iot47_wifi_ota.h"
```
# Khởi tạo
```
iot47_wifi_ota_begin(&server);
```
Đặt hàm này vào loop:
```
iot47_wifi_ota_loop();
```
Để vào trang upload firmware các bạn vào trình duyệt gõ 192.168.1.6/upload ( chú ý 192.168.1.6 là ip tùy chỉnh )
