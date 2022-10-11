from time import sleep
import paho.mqtt.client as mqtt
from SX127x.LoRa import *
import paho.mqtt.publish as publish
from SX127x.board_config import BOARD
import RPi.GPIO as GPIO
BOARD.setup()
GPIO.setwarnings(False)

Broker = "192.168.43.141"
sub_topic = "sensor/instructions"
sub_topic1 = "sensor1/instuctions"
sub_topic2 = "sensor2/instructions"
sub_topic3 = "sensor3/bme"
sub_topic4 = "sensor4/bme"
sub_topic5 = "sensor5/bme"
sub_topic6 = "sensor6/bme"
print("starting script")
class LoRaRcvCont(LoRa):
    def __init__(self, verbose=False):
        print("intializing.......")
        super(LoRaRcvCont, self).__init__(verbose)
        self.set_mode(MODE.SLEEP)
        self.set_dio_mapping([0] * 6)

    def start(self):
        print("startig lora in receive mode")
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)
        while True:
            sleep(.5)
            rssi_value = self.get_rssi_value()
            status = self.get_modem_status()
            sys.stdout.flush()

    def on_connect(client,userdata,flags,rc):
        print("Connected with result code"+str(rc))
        client.subscribe(sub_topic)
        client.subscribe(sub_topic1)

    def on_rx_done(self):
        print("\nReceived: ")
        self.clear_irq_flags(RxDone=1)
        payload = self.read_payload(nocheck=True)
        #print(bytes(payload).decode("utf-8",'ignore'))
        data=bytes(payload).decode("utf-8",'ignore')
        print(data)
        temp = (data[0:7])
        #temp1 = float(temp)
        humidity = (data[7:12])
        print("Temperature:")
        print(temp)
        print("Humidity")
        print(humidity)
        moisture = (data[12:18])
        print('Moisture')
        print(moisture)
        bmeT = (data[18:23])
        print('BMETEMP')
        print(bmeT)
        bmeP = (data[24:29])
        print('BMEPRES')
        print(bmeP)
        bmeA = (data[29:36])
        print('BMEALT')
        print(bmeA)
        bmeH = (data[36:42])
        print('BMEHUMID')
        print(bmeH)
        mqttc = mqtt.Client("node-red")
        mqttc.connect(Broker,1883,60)
        mqttc.publish(sub_topic,temp)
        mqttc.publish(sub_topic1,humidity)
        mqttc.publish(sub_topic2,moisture)
        mqttc.publish(sub_topic3,bmeT)
        mqttc.publish(sub_topic4,bmeP)
        mqttc.publish(sub_topic5,bmeA)
        mqttc.publish(sub_topic6,bmeH)
        self.set_mode(MODE.SLEEP)
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)
        sleep(5)


lora = LoRaRcvCont(verbose=False)
lora.set_mode(MODE.STDBY)
#  Medium Range  Defaults after init are 434.0MHz, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on 13 dBm
lora.set_pa_config(pa_select=1)
try:
    lora.start()
except KeyboardInterrupt:
    sys.stdout.flush()
    print("")
    sys.stderr.write("KeyboardInterrupt\n")
finally:
    sys.stdout.flush()
    print("")
    lora.set_mode(MODE.SLEEP)
    BOARD.teardown()

