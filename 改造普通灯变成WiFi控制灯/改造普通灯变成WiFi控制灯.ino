#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
 
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
 
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
 
void setup(void){
  Serial.begin(9600);   // 启动串口通讯
 
  pinMode(D4, OUTPUT); //设置内置LED引脚为输出模式以便控制LED
  
  //通过addAp函数存储  WiFi名称       WiFi密码
  wifiMulti.addAP("Dongzii", "dong010129");   //这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息。
  wifiMulti.addAP("dongzii", "dong010129");  //这3个WiFi网络名称和密码
  wifiMulti.addAP("嘉然今天生吃你妈", "qwertyui104");
  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 通过wifiMulti.run()，NodeMCU将会在当前环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU将会连接信号最强的那一个WiFi信号。一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。
    delay(1000);                              
    Serial.print(i++); Serial.print(' ');    
  }                                          
                                              
  
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP
 
  esp8266_server.begin();                           // 启动网站服务
  esp8266_server.on("/", HTTP_GET, handleRoot);     // 设置服务器根目录即'/'的函数'handleRoot'
  esp8266_server.on("/LED", HTTP_POST, handleLED);  // 设置处理LED控制请求的函数'handleLED'
  esp8266_server.onNotFound(handleNotFound);        // 设置处理404情况的函数'handleNotFound'
 
  Serial.println("HTTP esp8266_server started");//  显示网络服务功能已经启动
}
 
void loop(void){
  esp8266_server.handleClient();                     // 检查http服务器访问
}

void handleRoot() {       
  esp8266_server.send(200, "text/html", "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, minimum-scale=0.5, maximum-scale=2.0, user-scalable=yes\" /><title>Document</title><style>    *{     margin:0;     padding: 0;     font-family: \'微软雅黑\',sana-serif;     box-sizing:border-box;     }    body{     display:flex;     justify-content: center;      align-items: center;      background:rgba(0, 5, 12, 0.958);     min-height: 100vh;    }    form{      position: relative;     padding: 10px 30px;     margin:0 55px;        color: #21ebff;        text-decoration: none;        font-size: 20px;        text-transform: uppercase;        transition: 0.5s;        overflow: hidden;        -webkit-box-reflect:below 1px linear-gradient(transparent,#0003);    }    form:hover{      background:#21ebff;     color:#111;     box-shadow: 0 0 50px #21ebff;     transition-delay: 0.5s;    }    form:nth-child(1){      filter:hue-rotate(115deg);    }    form:nth-child(3){     filter:hue-rotate(270deg);    }    form::before{      content: \'\';      position: absolute;     top: 0;     left: 0;      width: 10px;      height: 10px;     border-top: 2px solid #21ebff;      border-left:2px solid #21ebff;      transition:0.5s;      transition-delay: 0.5s    }    form:hover::before{      width:100%;     height:100%;      transition-delay: 0s;    }    form::after{      content: \'\';      position: absolute;     right: 0;     bottom: 0;      width: 10px;      height: 10px;     border-bottom: 2px solid #21ebff;     border-right: 2px solid #21ebff;      transition:0.5s;      transition-delay: 0.5s;    }    form:hover::after{      width: 100%;      height: 100%;     transition-delay: 0s;    }</style></head><body><form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"open\" style=\"background-color: transparent;border-width:0px;outline:none;color: rgb(250, 243, 243);\"> </form></body></html>");//编写网页html代码进行页面美化
}
 
//处理LED控制请求的函数'handleLED'
void handleLED() {                          
  digitalWrite(D4,!digitalRead(D4));// 改变LED的点亮或者熄灭状态
  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转  
}
 
// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){
  esp8266_server.send(404, "text/plain", "404: Not found"); // 发送 HTTP 状态 404 (未找到页面) 并向浏览器发送文字 "404: Not found"
}
