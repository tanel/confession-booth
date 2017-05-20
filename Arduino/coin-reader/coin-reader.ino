
const int coinInt = 1;

int coinsChange = 0;

void setup() {
  Serial.begin(9600);
  attachInterrupt(coinInt, coinInserted, RISING);
}

void coinInserted() {
  coinsChange = 1;
}

void loop() {
  // check if coin inserted
  if (coinsChange == 1) {
    // unflag that a coin has been inserted
    coinsChange = 0;

    Serial.println("1");
  }

  delay(1000);
}

