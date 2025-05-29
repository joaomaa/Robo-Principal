// === PINOS ===
// Motores
#define pinMotor1 3
#define pinMotor2 5
#define pinMotor3 6
#define pinMotor4 9

// Sensores IR de borda
#define irFrontPin 7
#define irBackPin 8

// Sensor ultrassônico (HC–SR04)
#define trigPin 10
#define echoPin 11

void setup() {
  Serial.begin(9600);

  // Motores
  pinMode(pinMotor1, OUTPUT);
  pinMode(pinMotor2, OUTPUT);
  pinMode(pinMotor3, OUTPUT);
  pinMode(pinMotor4, OUTPUT);

  // Sensor(es) Infravermelho
  pinMode(irFrontPin, INPUT);
  pinMode(irBackPin, INPUT);

  // Ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration;
  float distancia;
  bool frontEdge = digitalRead(irFrontPin) == HIGH;
  bool backEdge  = digitalRead(irBackPin)  == HIGH;

  // 1) Borda (prioritário)
  if (frontEdge && !backEdge) {
    Serial.println("Sensor da frente achou borda");
    tras();
    delay(500);
    parada();
    return;
  }
  if (!frontEdge && backEdge) {
    Serial.println("Sensor de trás achou borda");
    frente();
    delay(500);
    parada();
    return;
  }

  // 2) Medir distância
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distancia = duration * 0.0343 / 2.0;  // em cm

  // 3) Busca normal
  if (distancia > 20.0 && !frontEdge && !backEdge) {
    Serial.println("procurando oponente");
    procura();
    delay(50);
    return;
  }

  // 4) Ataque usando frente()
  if (distancia > 0.0 && distancia <= 20.0 && !frontEdge && !backEdge) {
    Serial.println("achei o oponente! atacando...");
    // Enquanto o oponente ficar perto e não houver borda, segue em frente
    do {
      frente();

      // Recalcula distância
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distancia = duration * 0.0343 / 2.0;

      // Recheca borda antes de continuar
      frontEdge = digitalRead(irFrontPin) == HIGH;
      backEdge  = digitalRead(irBackPin)  == HIGH;

    } while (distancia <= 20.0 && !frontEdge && !backEdge);

    // Assim que o oponente for embora ou tiver borda, para e volta
    parada();
    delay(200);
    return;
  }

  // 5) Segurança
  parada();
  delay(50);
}

// ===== Métodos de movimentação =====

// Velocidade padrão (digitalWrite HIGH/LOW)
void frente() {
  digitalWrite(pinMotor1, HIGH);
  digitalWrite(pinMotor2, LOW);
  digitalWrite(pinMotor3, LOW);
  digitalWrite(pinMotor4, HIGH);
}

// Ré padrão
void tras() {
  digitalWrite(pinMotor1, LOW);
  digitalWrite(pinMotor2, HIGH);
  digitalWrite(pinMotor3, HIGH);
  digitalWrite(pinMotor4, LOW);
}

// Gira à esquerda padrão
void esquerda() {
  digitalWrite(pinMotor1, HIGH);
  digitalWrite(pinMotor2, LOW);
  digitalWrite(pinMotor3, LOW);
  digitalWrite(pinMotor4, LOW);
}

// Para todos os motores
void parada() {
  digitalWrite(pinMotor1, LOW);
  digitalWrite(pinMotor2, LOW);
  digitalWrite(pinMotor3, LOW);
  digitalWrite(pinMotor4, LOW);
}

// Busca em “quadrado”
void procura() {
  frente();
  delay(300);
  esquerda();
  delay(300);
}