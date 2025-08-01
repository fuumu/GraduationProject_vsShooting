#include "WebSocketClient.h"

// WebSocket Opcodes and constants for improved readability
const uint8_t OPCODE_BINARY = 0x02;
const uint8_t OPCODE_TEXT = 0x01;
const uint8_t OPCODE_PING = 0x09;
const uint8_t OPCODE_PONG = 0x0A;
const uint8_t OPCODE_CONNECTION_CLOSE = 0x08;

const uint8_t FRAME_FLAG_FIN = 0x80;
const uint8_t FRAME_FLAG_MASK = 0x80;
const unsigned long HANDSHAKE_TIMEOUT_MS = 5000; // 5 seconds timeout for handshake

// サーバーアドレスやポートなどを初期化
WebSocketClient::WebSocketClient(const char *serverAddr, uint16_t serverPort)
    : server(serverAddr), port(serverPort), connected(false), websocketKey("dGhlIHNhbXBsZSBub25jZQ==") {}

// デストラクタで通信を切断
WebSocketClient::~WebSocketClient()
{
    disconnect();
}

// サーバーに接続する
bool WebSocketClient::connectToServer()
{
    Serial.println("サーバーへの接続を試みます");
    // ipアドレスを元に指定のポートで接続
    if (client.connect(server.c_str(), port))
    {
        Serial.println("サーバーへの接続しました");

        // WebSocketハンドシェイクリクエスト送信
        client.println("GET / HTTP/1.1");
        client.print("Host: ");
        client.print(server);
        client.print(":");
        client.println(port);
        client.println("Upgrade: websocket");
        client.println("Connection: Upgrade");
        client.print("Sec-WebSocket-Key: ");
        client.println(websocketKey);
        client.println("Sec-WebSocket-Version: 13");
        client.println();

        // ハンドシェイクレスポンスをタイムアウト付きで待機
        unsigned long startTime = millis();
        bool handshakeComplete = false;
        while (millis() - startTime < HANDSHAKE_TIMEOUT_MS)
        {
            if (client.available())
            {
                // HTTPレスポンスのヘッダーの終わり（空行）を探す
                String line = client.readStringUntil('\n');
                Serial.println(line);
                if (line == "\r")
                {
                    handshakeComplete = true;
                    break;
                }
            }
        }

        if (handshakeComplete) {
            Serial.println("サーバーへの接続が確立されました");
            connected = true;
            return true;
        }
    }

    Serial.println("サーバーへの接続に失敗しました");
    disconnect(); // 接続失敗時は切断処理を呼ぶ
    return false;
}

// 通信を切断
void WebSocketClient::disconnect()
{
    if (client.connected())
        client.stop();
    connected = false;
}

// 接続されているかチェック
bool WebSocketClient::isConnected()
{
    return connected && client.connected();
}

void WebSocketClient::sendData(uint8_t x, uint8_t y, uint8_t life)
{
    // バイナリペイロード（3バイト）
    byte payload[3];

    // 追加 プレイヤー1の座標と残機を代入--------------------------------------
    payload[0] = x;
    payload[1] = y;
    payload[2] = life;

    int payloadLength = 3;

    // WebSocketバイナリフレーム
    client.write(FRAME_FLAG_FIN | OPCODE_BINARY);  // FIN=1, opcode=0x2 (binary frame)
    client.write(FRAME_FLAG_MASK | payloadLength); // MASK=1, payload length (3バイト)

    // [改善] RFC 6455では、クライアントからサーバーへの各フレームで新しいランダムなマスクキーを使用する必要があります。
    byte maskKey[4];
    // `randomSeed()`が事前に呼ばれていることを想定
    maskKey[0] = random(0, 256);
    maskKey[1] = random(0, 256);
    maskKey[2] = random(0, 256);
    maskKey[3] = random(0, 256);
    client.write(maskKey, 4);

    // ペイロードをマスクして送信
    for (int i = 0; i < payloadLength; i++)
    {
        client.write(payload[i] ^ maskKey[i % 4]);
    }

    Serial.print("バイナリメッセージを送信: x=");
    Serial.print(payload[0]);
    Serial.print(", y=");
    Serial.print(payload[1]);
    Serial.print(", life=");
    Serial.println(payload[2]);
}

bool WebSocketClient::receiveData(uint8_t &x, uint8_t &y, uint8_t &life)
{
    byte firstByte = 0;
    byte secondByte = 0;
    byte opcode = 0;
    int payloadLength = 0;

    if (client.available())
    {
        // WebSocketフレームを読み込み
        firstByte = client.read();
        secondByte = client.read();

        // フレームタイプを確認
        opcode = firstByte & 0x0F;

        // NOTE: この実装は125バイト以下のペイロード長にのみ対応しています。
        if (opcode == OPCODE_BINARY)
        { // バイナリフレーム
            payloadLength = secondByte & 0x7F;

            if (payloadLength >= 3 && client.available() >= 3)
            {
                x = client.read();
                y = client.read();
                life = client.read();

                Serial.print("バイナリメッセージを受信: x=");
                Serial.print(x);
                Serial.print(", y=");
                Serial.print(y);
                Serial.print(", life=");
                Serial.println(life);
            }

            // 残りのバイトを読み飛ばし
            while (payloadLength > 3 && client.available())
            {
                client.read();
                payloadLength--;
            }
        }
        else
        {
            // その他のフレームタイプ（テキスト、ping/pong等）
            Serial.print("未対応のフレームタイプ: 0x");
            Serial.println(opcode, HEX);

            payloadLength = secondByte & 0x7F;
            for (int i = 0; i < payloadLength && client.available(); i++)
            {
                client.read();
            }
        }
        return true;
    }
    return false;
}