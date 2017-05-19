
const int coinInt = 0;

volatile float coinsValue = 0.00;

int coinsChange = 0;

void setup() {
  Serial.begin(9600);
  attachInterrupt(coinInt, coinInserted, RISING);
  Serial.println("setup done");
}

void coinInserted() {
  coinsValue = coinsValue + 2;
  coinsChange = 1;
}

void loop() {
  // check if coin inserted
  if (coinsChange == 1) {
    // unflag that a coin has been inserted
    coinsChange = 0;

    Serial.print("Credit: $");
    Serial.println(coinsValue);
  }

  delay(1000);
}

