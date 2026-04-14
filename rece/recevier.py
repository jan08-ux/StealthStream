import paho.mqtt.client as mqtt
import ascon

key = bytes([0]*16)

def on_message(client, userdata, msg):
    try:
        hex_data = msg.payload.decode().strip()
        print("\nReceived:", hex_data)

        data = bytes.fromhex(hex_data)

        # 🔹 Split nonce + ciphertext
        nonce = data[:16]
        ciphertext = data[16:]

        # 🔹 CORRECT FUNCTION CALL
        plaintext = ascon.ascon_decrypt(
            key,
            nonce,
            b"",
            ciphertext,
            "Ascon-AEAD128"
        )

        if plaintext is None:
            print("❌ Authentication failed (tag mismatch)")
        else:
            print("✅ Decrypted:", plaintext.decode())

    except Exception as e:
        print("❌ Error:", repr(e))


client = mqtt.Client()
client.on_message = on_message

client.connect("broker.hivemq.com", 1883)
client.subscribe("ascon/s3")

print("Listening...")
client.loop_forever()
