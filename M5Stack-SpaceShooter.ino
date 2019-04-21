//======================== intro =======================================
//      Space Shooter, basically knock-off Space Invaders
//             and also maybe a bit of Galaga
//   Written by Tyler Edwards for the Hackerbox #0020 badge
//  But should work on any ESP32 and Adafruit ILI9341 display
//        I am sorry for the messy code, you'll just
//                  have to live with it
//      Tyler on GitHub: https://github.com/HailTheBDFL/
//          Hackerboxes: http://hackerboxes.com/
//=========================== setup ===================================
// Space Shooter with M5STACK : 2018.01.12 Transplant by macsbug
// Controller   : Buttons A = LEFT, B = RIGHT, C = START or SHOOTING
// Github:https://macsbug.wordpress.com/2018/01/12/esp32-spaceshooter-with-m5stack/
//===================================================================
#include <M5Stack.h>
//============================= game variables =========================
unsigned long offsetM = 0;
unsigned long offsetT = 0;
unsigned long offsetF = 0;
unsigned long offsetB = 0;
unsigned long offsetA = 0;
unsigned long offsetAF = 0;
unsigned long offsetAB = 0;
unsigned long offsetS = 0;
int threshold = 40;
boolean startPrinted = false;
boolean beginGame = false;
boolean beginGame2 = true;
boolean play = false;
int score = 0;
int scoreInc = 10;
int level = 1;
//---------------------Player---------------------------------------
int shipX = 147;
int shipY = 190;
int oldShipX = 0;
int oldShipY = 0;
int changeShipX = 0;
int changeShipY = 0;
int shipSpeed = 50;
boolean doSplode = false;
boolean fire = false;
int fFireX[5] = {0, 0, 0, 0, 0};
int fFireY[5] = {0, 0, 0, 0, 0};
int fFireAge[5] = {0, 0, 0, 0, 0};
//--------------------------Aliens----------------------------------
boolean alienLive[18];
int alienLiveCount = 18;
int alienX = 7;
int alienY = 25;
int oldAlienX = 7;
int oldAlienY = 25;
int changeAlienX = 6;
int changeAlienY = 0;
int alienSpeed = 200;
int oldAlienSpeed;
int aFireX[5];
int aFireY[5];
boolean aFireAge[5];
int chanceOfFire = 2;
//================================ bitmaps ========================
//your starship
const int shipImgW = 15;
const int shipImgH = 16;
char shipImg[] = "ZZZZZZZWZZZZZZZ"
				 "ZZZZZZZWZZZZZZZ"
				 "ZZZZZZZWZZZZZZZ"
				 "ZZZZZZWWWZZZZZZ"
				 "ZZZRZZWWWZZRZZZ"
				 "ZZZRZZWWWZZRZZZ"
				 "ZZZWZWWWWWZWZZZ"
				 "RZZWBWWRWWBWZZR"
				 "RZZBWWRRRWWBZZR"
				 "WZZWWWRWRWWWZZW"
				 "WZWWWWWWWWWWWZW"
				 "WWWWWWWWWWWWWWW"
				 "WWWWWRWWWRWWWWW"
				 "WWWZRRWWWRRZWWW"
				 "WWZZRRZWZRRZZWW"
				 "WZZZZZZWZZZZZZW";
//alien
const int alienImgW = 15;
const int alienImgH = 11;

char alienImg1[] = "ZBBZZZZYZZZZBBZ"
				   "ZZZBZYRYRYZBZZZ"
				   "ZZZZBRRYRRBZZZZ"
				   "ZZZZZYYYYYZZZZZ"
				   "ZZZZBBYYYBBZZZZ"
				   "ZZZBBBRRRBBBZZZ"
				   "ZZBBBZRRRZBBBZZ"
				   "ZBBBBZYYYZBBBBZ"
				   "ZBBBZZRRRZZBBBZ"
				   "BBBBZZZRZZZBBBB"
				   "BBBZZZZRZZZZBBB";

char alienImg2[] = "ZZZRZZBZBZZRZZZ"
				   "ZRRRZZBZBZZRRRZ"
				   "ZRRRZWRWRWZRRRZ"
				   "ZRRRZWWWWWZRRRZ"
				   "ZZRRRRWWWRRRRZZ"
				   "ZZZRRRBBBRRRZZZ"
				   "ZZRRRRBBBRRRRZZ"
				   "ZRRRRRWWWRRRRRZ"
				   "ZZRRRZBBBZRRRZZ"
				   "ZZZZRZBBBZRZZZZ"
				   "ZZZZRZZZZZRZZZZ";

char alienImg3[] = "ZZZZZZGZGZZZZZZ"
				  "ZZZGGRRGRRGGZZZ"
				  "ZZZZGRRGRRGZZZZ"
				  "ZZZZZGGGGGZZZZZ"
				  "ZZZGGYYGYYGGZZZ"
				  "GGGGGYYYYYGGGGG"
				  "ZGGGGYYYYYGGGGZ"
				  "ZGRGGZRZRZGGRGZ"
				  "GRRGZZZZZZZGRRG"
				  "GGGGZZZZZZZGGGG"
				  "ZGGZZZZZZZZZGGZ";

//flames
const int flamesImgW = 12;
const int flamesImgH = 6;
char flamesImg[] = "RZZZZZZZZZZRRZRYYRRYYRZR"
				   "RZRYYRRYYRZRZZRYRZZRYRZZ"
				   "ZZZRZZZZRZZZZZZRZZZZRZZZ";
//ship 'sploded
const int splodedImgW = 14;
const int splodedImgH = 16;
char splodedImg[] = "ZZZZZZWWZZZZZZZZZZRYWWYRZZZY"
					"ZZZRRWWRRRRZRWYZRRRRRYYRRRZW"
					"YZRYRYYRYYRRRZWWRYYYRYYYYYRZ"
					"WWRYYRYRYYYYRRWWRYYYRWRYBRRZ"
					"RRRYRRWWWRYRWZZRYYRRBBWRYRWW"
					"ZZRYYBBBRRYBWWRZZRYYYRRYYZZW"
					"ZRRWRYYRBYRZZWZZRYBRYYYYYRRZ"
					"ZRWWYYYWWRRRZZZZWRRWWRRRWZZZ";
 
TFT_eSprite myship = TFT_eSprite(&M5.Lcd);
TFT_eSprite myshipe = TFT_eSprite(&M5.Lcd);
TFT_eSprite myshipe2 = TFT_eSprite(&M5.Lcd);
TFT_eSprite alien1 = TFT_eSprite(&M5.Lcd);
TFT_eSprite alien2 = TFT_eSprite(&M5.Lcd);
TFT_eSprite alien3 = TFT_eSprite(&M5.Lcd);
TFT_eSprite aliene = TFT_eSprite(&M5.Lcd);
TFT_eSprite flames = TFT_eSprite(&M5.Lcd);
TFT_eSprite sploded = TFT_eSprite(&M5.Lcd);
TFT_eSprite lazer = TFT_eSprite(&M5.Lcd);
TFT_eSprite lazere = TFT_eSprite(&M5.Lcd);
TFT_eSprite lazer2 = TFT_eSprite(&M5.Lcd);

void makeSprite(TFT_eSprite* sprite, char img[], int imgW, int imgH) {
	uint16_t cellColor;
	sprite->createSprite(2 * imgW, 2 * imgH);
	sprite->fillSprite(BLACK);
	for (int i = 0; i < imgW * imgH; i++) {
		if (img[i] != 'Z') {
			switch (img[i]) {
				case 'W':
					cellColor = WHITE;
					break;
				case 'Y':
					cellColor = YELLOW;
					break;
				case 'B':
					cellColor = BLUE;
					break;
				case 'R':
					cellColor = RED;
					break;
				case 'G':
					cellColor = GREEN;
					break;
				default:
					cellColor = BLACK;
					break;
			}
			sprite->fillRect(2 * (i % imgW), 2 * (i / imgW), 2, 2, cellColor);
		}
	}
}

void eraseSprite(TFT_eSprite* sprite, int imgW, int imgH) {
	sprite->createSprite(imgW, imgH);
	sprite->fillSprite(BLACK);
}

//=============================== setup and loop ==================
void setup() {
	memset(alienLive, true, 18);
	memset(aFireX, 0, 5);
	memset(aFireY, 0, 5);
	memset(aFireAge, 0, 5);
	M5.begin();
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setTextColor(0x5E85);
	M5.Lcd.setTextSize(4);
	randomSeed(analogRead(6));
	pinMode(BUTTON_A_PIN, INPUT_PULLUP);
	pinMode(BUTTON_B_PIN, INPUT_PULLUP);
	pinMode(BUTTON_C_PIN, INPUT_PULLUP);
	makeSprite(&myship, shipImg, shipImgW, shipImgH);
	makeSprite(&alien1, alienImg1, alienImgW, alienImgH);
	makeSprite(&alien2, alienImg2, alienImgW, alienImgH);
	makeSprite(&alien3, alienImg3, alienImgW, alienImgH);
	makeSprite(&flames, flamesImg, flamesImgW, flamesImgH);
	makeSprite(&sploded, splodedImg, splodedImgW, splodedImgH);
	eraseSprite(&myshipe, 30, 44);
	eraseSprite(&myshipe2, 32, 44);
	eraseSprite(&aliene, 30, 22);
	eraseSprite(&lazere, 4, 4);
	lazer.createSprite(4, 4);
	lazer.fillSprite(MAGENTA);
	lazer2.createSprite(4, 3);
	lazer2.fillSprite(MAGENTA);
}
//==================================================================
void loop() {
	if (M5.BtnA.isPressed()) {
		left();
	}
	if (M5.BtnB.isPressed()) {
		right();
	}
	if (M5.BtnC.isPressed()) {
		select();
	}
	//-------------Start Screen--------------
	if (millis() - offsetS >= 900 and !beginGame) {
		if (!startPrinted) {
			M5.Lcd.setCursor(77, 105);
			M5.Lcd.print(">START<");
			startPrinted = true;
			offsetS = millis();
		} else {
			M5.Lcd.fillRect(77, 105, 244, 32, BLACK);
			startPrinted = false;
			offsetS = millis();
		}
	}
	if (beginGame and beginGame2) {
		M5.Lcd.fillRect(77, 105, 244, 32, BLACK);
		beginGame2 = false;
		play = true;
	}
	//-------------Player-----------------------------------------------
	if (millis() - offsetM >= shipSpeed and play) {
		moveShip();
		offsetM = millis();
	}
	if (oldShipX != shipX or oldShipY != shipY) {
		myshipe.pushSprite(oldShipX, oldShipY);
		oldShipX = shipX;
		oldShipY = shipY;
		myship.pushSprite(shipX, shipY);
	}
	if (fire and play) {
		fireDaLazer();
	}
	if (millis() - offsetB >= 50) {
		for (int i = 0; i < 5; i++) {
			if (fFireAge[i] < 20 and fFireAge[i] > 0){
				keepFirinDaLazer(i);
			}
			if (fFireAge[i] == 20) {
				stopFirinDaLazer(i);
			}
		}
		offsetB = millis();
	}  
	if (millis() - offsetT > 50) {
		changeShipX = 0;
		changeShipY = 0;
	}
	//---------------Aliens--------------------------------------------
	if (millis() - offsetA >= alienSpeed and play) {
		moveAliens();
		offsetA = millis();
	}
	if (findAlienX(5) >= 294){
		changeAlienX = -3;
		changeAlienY = 7;
	}
	if (alienX <= 6){
		changeAlienX = 3;
		changeAlienY = 7;
	}
	alienLiveCount = 0;
	for (int i = 0; i < 18; i++) {
		if (alienLive[i]) {
			alienLiveCount += 1;
			if (alienShot(i)) {
				aliene.pushSprite(findOldAlienX(i), findOldAlienY(i));
				alienLiveCount -= 1;
				alienLive[i] = false;
				score += scoreInc;
			}
			if (onPlayer(i) or exceedBoundary(i)) {
				gameOver();
			}
		}
	}
	if (alienLiveCount == 1) {
		oldAlienSpeed = alienSpeed;
		if (alienSpeed > 50) {
			alienSpeed -= 10;
		} else {
			alienSpeed = 20;
		}
	}
	if (alienLiveCount == 0) {
		levelUp();
	}
	M5.update();
}
// functions =======================================================
void gameOver() {
	play = false;
	if (doSplode) {
		sploded.pushSprite(shipX, shipY);
	}
	M5.Lcd.fillScreen(BLACK);
	drawScore(false);
	delay(1000);
	M5.Lcd.setCursor(17, 168);
	M5.Lcd.setTextSize(2);
	M5.Lcd.print("(Reset device to replay)");
	while (1) { }
}
//==================================================================
void drawScore(boolean win) {
	M5.Lcd.setCursor(53, 40);
	M5.Lcd.setTextColor(WHITE);
	M5.Lcd.setTextSize(4);
	if (win) {
		M5.Lcd.print("LEVEL UP!");
	} else {
		M5.Lcd.print("GAME OVER");
	}
	delay(1000);
	M5.Lcd.setCursor(59, 89);
	M5.Lcd.setTextSize(3);
	M5.Lcd.print("Score: "); M5.Lcd.print(score);
	offsetM = millis();
	delay(1000);
	M5.Lcd.setCursor(71, 128);
	M5.Lcd.print("Level: "); M5.Lcd.print(level);
}
//==================================================================
void levelUp() {
	play = false;
	memset(alienLive, true, 18);
	memset(aFireX, 0, 5);
	memset(aFireY, 0, 5);
	memset(aFireAge, 0, 5);
	alienX = 7;
	alienY = 25;
	oldAlienX = 7;
	oldAlienY = 25;
	alienSpeed = oldAlienSpeed;
	if (alienSpeed > 100) {
		alienSpeed -= 10; chanceOfFire -= 10;
	} else if (alienSpeed > 50) {
		alienSpeed -= 10; chanceOfFire -=5;
	} else if (alienSpeed > 25) {
		alienSpeed -= 5; chanceOfFire -=1;
	}
	score += 50;
	scoreInc += 5;
	changeShipX = 0;
	changeShipY = 0; 
	for (unsigned long i = millis(); millis() - i <= 1600;) {
		if (millis() - offsetM >= 20) {
			myshipe2.pushSprite(oldShipX, oldShipY);
			myship.pushSprite(shipX, shipY);
			flames.pushSprite(shipX + 1, shipY + 32);
			oldShipX = shipX; oldShipY = shipY;
			shipY -= 6;
			offsetM = millis();
		}
	}
	drawScore(true);
	level += 1;
	shipX = 147;
	shipY = 190;
	delay(4000);
	M5.Lcd.fillScreen(BLACK);
	offsetM = millis();
	play = true;
}
//==================================================================
boolean alienShot(int num) {
	for (int i; i < 5; i++) {
		if (fFireAge[i] < 20 and fFireAge[i] > 0) {
			if (fFireX[i] > findAlienX(num) - 4 and fFireX[i] < findAlienX(num) + 28 and fFireY[i] < findAlienY(num) + 22 and fFireY[i] > findAlienY(num) + 4) {
				fFireAge[i] = 20;
				return true;
			}
		}
	}
	return false;
}
//==================================================================
boolean onPlayer(int num) {
	if (findAlienX(num) - shipX < 24 and findAlienX(num) - shipX > -28 and findAlienY(num) - shipY < 32 and findAlienY(num) - shipY > -22) {
		doSplode = true;
		return true;
	} else {
		return false;
	}
}
//==================================================================
boolean exceedBoundary(int num) {
	if (findAlienY(num) > 218) {
		return true;
	} else {
		return false;
	}
}
//==================================================================
void moveAliens() {
	for (int i = 0; i < 18; i++) {
		if (alienLive[i]) {
			aliene.pushSprite(findOldAlienX(i), findOldAlienY(i));
			if ( i < 6 ) {
				alien3.pushSprite(findAlienX(i), findAlienY(i));
			} else if ( i < 12 ) {
				alien2.pushSprite(findAlienX(i), findAlienY(i));
			} else {
				alien1.pushSprite(findAlienX(i), findAlienY(i));
			}
		}
	}
	oldAlienX = alienX; oldAlienY = alienY;
	alienX += changeAlienX; alienY += changeAlienY;
	if (changeAlienY != 0) {
		changeAlienY = 0;
	}
}
//==================================================================
int findAlienX(int num) {
	return alienX + 42*(num % 6);
}
//==================================================================
int findAlienY(int num) {
	return alienY + 33*(num / 6);
}
//==================================================================
int findOldAlienX(int num) {
	return oldAlienX + 42*(num % 6);
}
//==================================================================
int findOldAlienY(int num) {
	return oldAlienY + 33*(num / 6);
}
//---------------------------Player---------------------------------
void fireDaLazer() {
	int bulletNo = -1;
	for (int i = 0; i < 4; i++) {
		if (fFireAge[i] == 0) { bulletNo = i;}
	}
	if (bulletNo != -1) {
		fFireAge[bulletNo] = 1;
		fFireX[bulletNo] = shipX + 13;
		fFireY[bulletNo] = shipY - 4;
		lazer2.pushSprite(fFireX[bulletNo], fFireY[bulletNo]);
	}
	fire = false;
}
//==================================================================
void keepFirinDaLazer(int bulletNo) {
	lazere.pushSprite(fFireX[bulletNo], fFireY[bulletNo]);
	fFireY[bulletNo] -= 8;
	lazer.pushSprite(fFireX[bulletNo], fFireY[bulletNo]);
	fFireAge[bulletNo] += 1;
}
//==================================================================
void stopFirinDaLazer(int bulletNo) {
	lazere.pushSprite(fFireX[bulletNo], fFireY[bulletNo]);
	fFireAge[bulletNo] = 0;
}
//==================================================================
void moveShip() {
	if (shipX + changeShipX < 288 and shipX + changeShipX > 6 and changeShipX != 0) {
		shipX += changeShipX;
	}
	if (shipY + changeShipY > 24 and shipY + changeShipY < 192 and changeShipY != 0) {
		shipY += changeShipY;
	}
	if (oldShipX != shipX or oldShipY != shipY) {
		myshipe2.pushSprite(oldShipX, oldShipY);
		oldShipX = shipX; oldShipY = shipY;
		myship.pushSprite(shipX, shipY);
	}
}
//=========================== button functions =====================
void up() {
	if (millis() - offsetT >= 50 and play) {
		changeShipX = 0; changeShipY = -6; offsetT = millis();
	}
}
//==================================================================
void down() {
	if (millis() - offsetT >= 50 and play) {
		changeShipX = 0; changeShipY = 6; offsetT = millis();
	}
}
//==================================================================
void left() {
	if (millis() - offsetT >= 50 and play) {
		changeShipX = -6; changeShipY = 0; offsetT = millis();
	}
}
//==================================================================
void right() {
	if (millis() - offsetT >= 50 and play) {
		changeShipX = 6; changeShipY = 0; offsetT = millis();
	}
}
//==================================================================
void select() {
	if (millis() - offsetF >= 500 and play) {
		fire = true;
		offsetF = millis();
	}
	if (!beginGame) {
		beginGame = true;
	}
}
//==================================================================
