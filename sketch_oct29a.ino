#define SAMPLES 128  // Number of samples
#define SAMPLING_FREQUENCY 1000  // Sampling frequency in Hz

int real[SAMPLES];
int imag[SAMPLES];

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < SAMPLES; i++) {
    // Create a simple test signal (sine wave)
    real[i] = 512 * sin(2 * PI * 50 * i / SAMPLING_FREQUENCY);
    imag[i] = 0;
  }

  // Perform Discrete Fourier Transform
  for (int k = 0; k < SAMPLES; k++) {
    int realSum = 0;
    int imagSum = 0;

    for (int n = 0; n < SAMPLES; n++) {
      int angle = 2 * PI * k * n / SAMPLES;
      realSum += real[n] * cos(angle) + imag[n] * sin(angle);
      imagSum += imag[n] * cos(angle) - real[n] * sin(angle);
    }

    real[k] = realSum / SAMPLES;
    imag[k] = imagSum / SAMPLES;

    Serial.print(real[k]);  // Output real part
    Serial.print(" ");
    Serial.println(imag[k]);  // Output imaginary part
  }

  delay(1);  // Delay between DFT calculations
}
