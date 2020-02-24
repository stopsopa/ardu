


// http://www.circuitstoday.com/arduino-soil-moisture-sensor

// Sketch uses 4204 bytes (13%) of program storage space. Maximum is 30720 bytes.
// Global variables use 282 bytes (13%) of dynamic memory, leaving 1766 bytes for local variables. Maximum is 2048 bytes.

/***************************************************
 This example reads Capacitive Soil Moisture Sensor.
 
 Created 2015-10-21
 By berinie Chen <bernie.chen@dfrobot.com>
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/





int sensor_pin = A0;

int output_value ;

void setup() {

   Serial.begin(9600);

   Serial.println("Reading From the Sensor ...");

   delay(500);
}

//int out = 1023; // after autocalibration
int out = 1023;

// it can reach value around 196 if suddenly placed in water
//int in = 195; // after autocalibration
int in = 195;

// the purpose of autocalibration is to find most extreme values
// in air and in the water
// WARNING: zero this number ON PRODUCTION to stop allowing autocalibration
int maxcalibratepoints = 60;

// -----------

int outx = out;

int inx = in;

int mapped = 0;

int raw = 0;

int mcx = maxcalibratepoints;

void loop() {

   raw = analogRead(sensor_pin);

   if (raw < inx && mcx > 0) {

      Serial.println("inx -1");

      inx -= 1;

      mcx -= 1;
   }

   if (raw > outx && mcx > 0) {

      Serial.println("outx +1");

      outx += 1;

      mcx -= 1;
   }

   mapped = map(raw, outx, inx, 0, 100);

   Serial.print((String)"raw:" + raw + " calc:" + mapped + "% inx:" + in + "->" + inx + " outx:" + out + "->" + outx + " mc:" + maxcalibratepoints + "->" + mcx);

   Serial.println("");

   delay(1000);

 }
