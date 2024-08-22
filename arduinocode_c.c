#include <LiquidCrystal.h> //LCD ekrana ait komutları kullanabilmek için kütüphanesini ekliyoruz.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //Arduinonun çeşitli pinlerini isimlendiriyoruz.
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //İsimlendirdiğimiz pinleri LCD ekranın pinlerine atıyoruz.


#include <Stepper.h> //Step motora ait komutları kullanabilmek için kütüphanesini ekliyoruz.
const int stepsPerRevolution = 20; //Step motorun 360 derece dönmesi için atması gereken adım sayısı. Biz adım açısı 18 derecelik step motor kullandığımız için 360/18=20 değerini girdik.
Stepper stepperA(stepsPerRevolution , A1, A2, A3, A4); //Step motora ait pinleri giriyoruz.
Stepper stepperB(stepsPerRevolution , 6, 7, 8, 9); //Step motora ait pinleri giriyoruz.
int stepperCounter = 0; //Defining step motor turning number to fix( it will return 2 times) later.

const int LED = 13; //we are givin name for a LED's pin which we are using.

String masterPassword = "1234";  // determining default passwords.
String APassword = "2222";
String BPassword = "3333";
String baitPassword = "9999";

bool returnMainMenu = true;
bool masterLockScreen = false;
bool timeLockScreen = false;
String password = "";
String onLCD;
String LCDClear = "                ";

int passwordCounter = 0;
bool isPasswordCorrect = false;

int insideTheSectionA = 0;
int runOnceTheSectionA = 1;
int insideTheSectionB = 0;
int runOnceTheSectionB = 1;
int insideChangePassword = 0;
int runOnceChangePassword = 1;
int insideReport = 0;
int runOnceReport = 1;
int insideTheMasterLock = 0;

int typingPassword = 0;
int typingSubMenu = 0;
int subMenuNumber = 0;

int SFAA = 0; //Successive Failure Attempts for Section A
int SFAB = 0; //Successive Failure Attempts for Section B
int SFACP = 0; //Successive Failure Attempts for Section Change Password

int SA = 0; //Total number of succesful attempts
int USA = 0; //Total number of unsuccesful attemps

int counter = 0;
bool isCountDownEnded = false;

bool isSectionAOpen = true;
bool isSectionBOpen = true;

void setup() {
  pinMode(LED, OUTPUT); //LED isimli pini çıkış moduna alıyoruz.
  pinMode(A0, INPUT); //A0 isimli pini giriş moduna alıyoruz.
  pinMode(A1, OUTPUT); //A1 isimli pini çıkış moduna alıyoruz. A1,A2,A3,A4 are pins of StepMotorA.
  pinMode(A2, OUTPUT); //A2 isimli pini çıkış moduna alıyoruz.
  pinMode(A3, OUTPUT); //A3 isimli pini çıkış moduna alıyoruz.
  pinMode(A4, OUTPUT); //A4 isimli pini çıkış moduna alıyoruz.
  pinMode(6, OUTPUT); // 6 isimli pini çıkış moduna alıyoruz. 6,7,8,9 are pins of StepMotorB.
  pinMode(7, OUTPUT); // 7 isimli pini çıkış moduna alıyoruz.
  pinMode(8, OUTPUT); // 8 isimli pini çıkış moduna alıyoruz.
  pinMode(9, OUTPUT); // 9 isimli pini çıkış moduna alıyoruz.

  
  lcd.begin(16, 2); //Starting our LCD screen and determining its size.
  lcd.setCursor(0, 0); //İmleci ilk satırın başına alıyoruz.
  lcd.print("Main Menu"); //Main screen
  lcd.setCursor(0, 1); //İmleci ikinci satırın başına alıyoruz.
  lcd.print("Enter 1,2,3 or 4"); //İkinci satırı boş bırakıyoruz.

  stepperA.setSpeed(60);
  stepperB.setSpeed(60);

  for(int stepperCounter=0; stepperCounter<2; stepperCounter++) //2 times turning.
  {
    stepperA.step(-stepsPerRevolution); //Section A oppening.
    isSectionAOpen = true;
    stepperB.step(-stepsPerRevolution); //Section B opening.
    isSectionBOpen = true;
  }
  
}

void loop() 
{
  if(returnMainMenu == true) //function of returning main menu
  {
    lcd.clear();
    lcd.setCursor(0, 0); //İmleci ilk satırın başına alıyoruz.
    lcd.print("Main Menu"); //main screen
    lcd.setCursor(0, 1); //İmleci ikinci satırın başına alıyoruz.
    lcd.print("Enter 1,2,3 or 4");
    returnMainMenu = false;
    runOnceReport = 1;
  }

  if(masterLockScreen == true) //function of returning masterlock screen
  {
    lcd.clear();
    lcd.setCursor(0, 0); //İmleci ilk satırın başına alıyoruz.
    lcd.print("System Locked MP");
    lcd.setCursor(0, 1); //İmleci ikinci satırın başına alıyoruz.
    lcd.print("Enter MP");
    masterLockScreen = false;
    typingPassword = 1;
    insideTheSectionA = 0;
    insideTheSectionB = 0;
    insideChangePassword = 0;
    insideTheMasterLock = 1;
    runOnceReport = 1;
  }

  if(timeLockScreen == true)
  {
    counter = 20;

    while(counter != 0 && counter < 21)
    {
      counter --;
      lcd.clear();
      lcd.setCursor(0, 0); //İmleci ilk satırın başına alıyoruz.
      onLCD = (String("No Access ") + String(counter) + String(" sec"));
      lcd.print(onLCD); //Ana ekran
      lcd.setCursor(0, 1); //İmleci ikinci satırın başına alıyoruz.
      lcd.print(LCDClear);
      delay(1000);
      if(counter == 0)
      {
        isCountDownEnded = true;
      }
    }
    if(isCountDownEnded == true)
    {
      timeLockScreen = false;
      masterLockScreen = true;
      onLCD = "";
      password = "";
      passwordCounter = 0;
      typingPassword = 0;
    }
  }
  
  //NumberPad Main Menu
  int buttonResistance = analogRead(A0); //Butona basıldığında A0 üzerinden direnç değerini okuyoruz.
  if(SFAA != 0 && SFAA <3)
  {
    if(isPasswordCorrect == true)
    {
      
    }
    else
    {
      buttonResistance = 155;
    }
  }
  if(SFAB != 0 && SFAB <3)
  {
    if(isPasswordCorrect == true)
    {
      
    }
    else
    {
      buttonResistance = 245;
    }
  }
  if(SFACP != 0 && SFACP <3)
  {
    if(isPasswordCorrect == true)
    {
      
    }
    else
    {
      buttonResistance = 345;
    }
    
  }
  if(buttonResistance > 150 && buttonResistance < 160 && insideTheSectionA == 0 && typingPassword == 0) //Kullanıcı 1'e bastığı için Section A'ya giriyoruz.
  {
    //User entered "1" to enter sectionA.
    insideTheSectionA = 1;
    if(runOnceTheSectionA == 1)
    {
      //Password screen oppening to enter sectionA.
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Open/Lock Sec.A");
      lcd.setCursor(0, 1);
      lcd.print("Enter Pass.A");
      delay(100);
      runOnceTheSectionA = 0;
      typingPassword = 1;
    }
    delay(200);
  }
  if(buttonResistance > 240 && buttonResistance < 250 && insideTheSectionB == 0 && typingPassword == 0) //Kullanıcı 2'ye bastığı için Section B'ye giriyoruz.
  {
    //Kullanıcı ana menüde Section B'ye girmek için 2'ye bastı
    insideTheSectionB = 1;
    if(runOnceTheSectionB == 1)
    {
      //Oppening screen coming to enter sectionB.
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Open/Lock Sec.B");
      lcd.setCursor(0, 1);
      lcd.print("Enter Pass.B");
      delay(100);
      runOnceTheSectionB = 0;
      typingPassword = 1;
    }
    delay(200);
  }
  if(buttonResistance > 340 && buttonResistance < 350 && insideChangePassword == 0 && typingPassword == 0) //Kullanıcı 3'e bastığı için Şifre değiştirme ekranına giriyoruz.
  {
    //Kullanıcı ana menüde "Şifre Değiştirme Ekranı"na girmek için 3'e bastı
    insideChangePassword = 1;
    if(runOnceChangePassword == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Master P.for CP");
      lcd.setCursor(0, 1);
      lcd.print("Enter MasterP.");
      delay(100);
      runOnceChangePassword = 0;
      typingPassword = 1;
    }
    delay(200);
  }
  if(buttonResistance > 190 && buttonResistance < 200 && insideChangePassword == 0 && typingPassword == 0) //Kullanıcı 4'e bastığı için rapor verme ekranına giriyoruz.
  {
    //Kullanıcı ana menüde "Rapor Verme Ekranı"na girmek için 4'e bastı
    insideReport = 1;
    if(runOnceReport == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      String toLCD1 = String(SA) + String(" Suc. At.");
      lcd.print(toLCD1);
      lcd.setCursor(0, 1);
      String toLCD2 = String(USA) + String(" Unsuc. At.");
      lcd.print(toLCD2);
      delay(100);
      runOnceReport = 0;
      //typingPassword = 1;
      
    }
    delay(2000);
    returnMainMenu = true;
  }

  while(typingPassword == 1)
  {
    //NumberPad Password
    int buttonResistance2 = analogRead(A0);
    if(buttonResistance2 > 150 && buttonResistance2 < 160) //Kullanıcı şifresini girmek için 1 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "1";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    if(buttonResistance2 > 240 && buttonResistance2 < 250) //Kullanıcı şifresini girmek için 2 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "2";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    if(buttonResistance2 > 340 && buttonResistance2 < 350) //Kullanıcı şifresini girmek için 3 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "3";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    if(buttonResistance2 > 190 && buttonResistance2 < 200) //Kullanıcı şifresini girmek için 4 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "4";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    if(buttonResistance2 > 310 && buttonResistance2 < 320) //Kullanıcı şifresini girmek için 5 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "5";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    if(buttonResistance2 > 410 && buttonResistance2 < 420) //Kullanıcı şifresini girmek için 6 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "6";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    if(buttonResistance2 > 540 && buttonResistance2 < 560) //Kullanıcı şifresini girmek için 7 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "7";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    if(buttonResistance2 > 690 && buttonResistance2 < 700) //Kullanıcı şifresini girmek için 8 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "8";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    if(buttonResistance2 > 780 && buttonResistance2 < 790) //Kullanıcı şifresini girmek için 9 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "9";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    if(buttonResistance2 > 930 && buttonResistance2 < 940) //Kullanıcı şifresini girmek için 0 tuşuna bastı
    {
      if(passwordCounter < 4)
      {
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        lcd.setCursor(0, 1);
        onLCD = onLCD + "*";
        lcd.print(onLCD);
        password = password + "0";
        passwordCounter = passwordCounter + 1;
        delay(1);
        }
    }
    delay(200);
    if(passwordCounter == 4)
    {
      typingPassword = 0;
    }
  }
    
    if(passwordCounter == 4) //Kullanıcı 4 haneli şifresini girdi
    {
      if(password == masterPassword && insideTheMasterLock == 1)
      {
        typingPassword = 0;
        insideTheMasterLock = 0;
        returnMainMenu = true;
        onLCD = "";
        password = "";
        passwordCounter = 0;
        SA = SA + 1;
      }
      if(password != masterPassword && insideTheMasterLock == 1)
      {
        typingPassword = 0;
        insideTheMasterLock = 0;
        returnMainMenu = false;
        timeLockScreen = true;
        onLCD = "";
        password = "";
        passwordCounter = 0;
        USA = USA + 1;
        
      }
      if(password == masterPassword && insideTheSectionA == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sec.A Opening/Closing");
        lcd.setCursor(0, 1);
        lcd.print("MasterPass.");
        typingPassword = 0;
        isPasswordCorrect = true;
        SA = SA + 1;
        
        if(isSectionAOpen == true)
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperA.step(stepsPerRevolution);
          delay(200);
        }
        isSectionAOpen = false;
        }
        else
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperA.step(-stepsPerRevolution);
          delay(200);
        }
        isSectionAOpen = true;
        }
        
      }
      if(password == masterPassword && insideTheSectionB == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sec.B Opening/Closing");
        lcd.setCursor(0, 1);
        lcd.print("MasterPass.");
        typingPassword = 0;
        isPasswordCorrect = true;
        SA = SA + 1;

        if(isSectionBOpen == true)
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperB.step(stepsPerRevolution);
          delay(200);
        }
        isSectionBOpen = false;
        }
        else
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperB.step(-stepsPerRevolution);
          delay(200);
        }
        isSectionBOpen = true;
        }
        
      }
      if(password == masterPassword && insideChangePassword == 1) // Password is true , so we are asking which password will be change. 
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CP 1M 2A 3B 4T");
        lcd.setCursor(0, 1);
        lcd.print(LCDClear);
        typingPassword = 0;
        typingSubMenu = 1;
        returnMainMenu = false;
        delay(100);
        isPasswordCorrect = true;
        SA = SA + 1;
      }
      if(password == APassword && insideTheSectionA == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sec.A Opening/Closing");
        lcd.setCursor(0, 1);
        lcd.print("APass.");
        typingPassword = 0;
        isPasswordCorrect = true;
        SA = SA + 1;

        if(isSectionAOpen == true)
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperA.step(stepsPerRevolution);
          delay(200);
        }
        isSectionAOpen = false;
        }
        else
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperA.step(-stepsPerRevolution);
          delay(200);
        }
        isSectionAOpen = true;
        }
        
      }
      if(password == BPassword && insideTheSectionB == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sec.B Opening/Closing");
        lcd.setCursor(0, 1);
        lcd.print("BPass.");
        typingPassword = 0;
        isPasswordCorrect = true;
        SA = SA + 1;

        if(isSectionBOpen == true)
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperB.step(stepsPerRevolution);
          delay(200);
        }
        isSectionBOpen = false;
        }
        else
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperB.step(-stepsPerRevolution);
          delay(200);
        }
        isSectionBOpen = true;
        }
        
      }
      if(password == baitPassword && insideTheSectionA == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sec.A Opening/Closing");
        lcd.setCursor(0, 1);
        lcd.print("Bait");
        typingPassword = 0;
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
        isPasswordCorrect = true;
        SA = SA + 1;

        if(isSectionAOpen == true)
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperA.step(stepsPerRevolution);
          delay(200);
        }
        isSectionAOpen = false;
        }
        else
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperA.step(-stepsPerRevolution);
          delay(200);
        }
        isSectionAOpen = true;
        }
        
      }
      if(password ==  baitPassword && insideTheSectionB == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sec.B Opening/Closing");
        lcd.setCursor(0, 1);
        lcd.print("Bait");
        typingPassword = 0;
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
        isPasswordCorrect = true;
        SA = SA + 1;
        
        if(isSectionBOpen == true)
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperB.step(stepsPerRevolution);
          delay(200);
        }
        isSectionBOpen = false;
        }
        else
        {
          for(int stepperCounter=0; stepperCounter<2; stepperCounter++)
        {
          stepperB.step(-stepsPerRevolution);
          delay(200);
        }
        isSectionBOpen = true;
        }
        
      }
      if(password != masterPassword && password != APassword && password != baitPassword && insideTheSectionA == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong Password");
        lcd.setCursor(0, 1);
        lcd.print(password);
        SFAA = SFAA + 1;
        isPasswordCorrect = false;
        USA = USA + 1;
      }
      if(password != masterPassword && password != BPassword && password != baitPassword && insideTheSectionB == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong Password");
        lcd.setCursor(0, 1);
        lcd.print(password);
        SFAB = SFAB + 1;
        isPasswordCorrect = false;
        USA = USA + 1;
      }
      if(password != masterPassword && insideChangePassword == 1)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong Password");
        lcd.setCursor(0, 1);
        lcd.print(password);
        SFACP = SFACP +1;
        isPasswordCorrect = false;
        USA = USA + 1;
      }
      
      if(insideTheSectionA == 1)
      {
        runOnceTheSectionA = 1;
        insideTheSectionA = 0;
        returnMainMenu = true;
        onLCD = "";
        password = "";
        passwordCounter = 0;
        typingPassword = 0;
      }
      
      if(insideTheSectionB == 1)
      {
        runOnceTheSectionB = 1;
        insideTheSectionB = 0;
        returnMainMenu = true;
        onLCD = "";
        password = "";
        passwordCounter = 0;
        typingPassword = 0;
      }
      if(insideChangePassword == 1)
      {
        runOnceChangePassword = 1;
        insideChangePassword = 0;
        returnMainMenu = true;
        onLCD = "";
        password = "";
        passwordCounter = 0;
      }
      
      if(SFAA != 0 && SFAA <3)
      {
        if(isPasswordCorrect == true)
        {
          returnMainMenu = true;
        }
        else
        {
          returnMainMenu = false;
        }
      }
      if(SFAA == 3 || SFAA >3)
      {
        SFAA = 0;
        masterLockScreen = true;
       
      }
      if(SFAB != 0 && SFAB <3)
      {
        if(isPasswordCorrect == true)
        {
          returnMainMenu = true;
        }
        else
        {
          returnMainMenu = false;
        }
      }
      if(SFAB == 3 || SFAB >3)
      {
        SFAB = 0;
        masterLockScreen = true;
       
      }
      if(SFACP != 0 && SFACP <3)
      {
        if(isPasswordCorrect == true)
        {
          returnMainMenu = true;
        }
        else
        {
          returnMainMenu = false;
        }
      }
      if(SFACP == 3 || SFACP >3)
      {
        SFACP = 0;
        masterLockScreen = true;
        
      }

      if(typingSubMenu == 0)
      {
        if(subMenuNumber == 1)
        {
          masterPassword = password;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("M Pass Changed");
          lcd.setCursor(0, 1);
          lcd.print(password);
          typingSubMenu = 0;
          subMenuNumber = 0;
          returnMainMenu = true;
          typingPassword = 0;
          insideTheSectionA = 0;
          insideTheSectionB = 0;
          insideChangePassword = 0;
          onLCD = "";
          password = "";
          passwordCounter = 0;
          delay(1);
        }
        if(subMenuNumber == 2)
        {
          APassword = password;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("A Pass Changed");
          lcd.setCursor(0, 1);
          lcd.print(password);
          typingSubMenu = 0;
          subMenuNumber = 0;
          returnMainMenu = true;
          typingPassword = 0;
          insideTheSectionA = 0;
          insideTheSectionB = 0;
          insideChangePassword = 0;
          onLCD = "";
          password = "";
          passwordCounter = 0;
          delay(1);
        }
        if(subMenuNumber == 3)
        {
          BPassword = password;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("B Pass Changed");
          lcd.setCursor(0, 1);
          lcd.print(password);
          typingSubMenu = 0;
          subMenuNumber = 0;
          returnMainMenu = true;
          typingPassword = 0;
          insideTheSectionA = 0;
          insideTheSectionB = 0;
          insideChangePassword = 0;
          onLCD = "";
          password = "";
          passwordCounter = 0;
          delay(1);
        }
        if(subMenuNumber == 4)
        {
          baitPassword = password;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("T Pass Changed");
          lcd.setCursor(0, 1);
          lcd.print(password);
          typingSubMenu = 0;
          subMenuNumber = 0;
          returnMainMenu = true;
          typingPassword = 0;
          insideTheSectionA = 0;
          insideTheSectionB = 0;
          insideChangePassword = 0;
          onLCD = "";
          password = "";
          passwordCounter = 0;
          delay(1);
        }
      }
      
      //NumberPad Sub Menu
      while(typingSubMenu == 1)
      {
        int buttonResistance3 = analogRead(A0);
        if(buttonResistance3 > 150 && buttonResistance3 < 160) //Kullanıcı alt menü seçmek için 1 tuşuna bastı.
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter New Pass M");
          lcd.setCursor(0, 1);
          lcd.print(LCDClear);
          subMenuNumber = 1;
          typingSubMenu = 0;
          typingPassword = 1;
          delay(1);
          returnMainMenu = false;
        }
        if(buttonResistance3 > 240 && buttonResistance3 < 250) //Kullanıcı alt menü seçmek için 2 tuşuna bastı.
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter New Pass A");
          lcd.setCursor(0, 1);
          lcd.print(LCDClear);
          subMenuNumber = 2;
          typingSubMenu = 0;
          typingPassword = 1;
          delay(1);
          returnMainMenu = false;
        }
        if(buttonResistance3 > 340 && buttonResistance3 < 350) //Kullanıcı alt menü seçmek için 3 tuşuna bastı.
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter New Pass B");
          lcd.setCursor(0, 1);
          lcd.print(LCDClear);
          subMenuNumber = 3;
          typingSubMenu = 0;
          typingPassword = 1;
          delay(1);
          returnMainMenu = false;
        }
        if(buttonResistance3 > 190 && buttonResistance3 < 200) //Kullanıcı alt menü seçmek için 4 tuşuna bastı.
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter New Pass T");
          lcd.setCursor(0, 1);
          lcd.print(LCDClear);
          subMenuNumber = 4;
          typingSubMenu = 0;
          typingPassword = 1;
          delay(1);
          returnMainMenu = false;
        }
        
      }


      delay(2000);
    }
     
}