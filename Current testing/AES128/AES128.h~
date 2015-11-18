
AES aes ;

byte key[] = 
{
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;

byte my_iv[] = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
} ; // this is the iv value that is passed between app and lock in plain text along with the encrypted data

char PassString[] = "This is hard to believe but true"; // this should be the secret the lock and the app should know. later we will think of how to arrange this secret key.
byte plain[] = "We, the Fairies, blithe and antic Of dimensions not gigantic, Th"; // this is the data that needs to be protected

byte cipher [4*N_BLOCK] ;
byte check [4*N_BLOCK] ;

int bits = 128;
int blocks = 4;

String encrypt(String s) 
{
  byte iv [N_BLOCK] ;
  // Set Key
  byte succ = aes.set_key (key, bits) ;
  Serial.println ("Starting encryption...") ;
  // Encrypt
  for (byte i = 0 ; i < 16 ; i++)
    iv[i] = my_iv[i] ;
  succ = aes.cbc_encrypt (plain, cipher, blocks, iv) ;
}

String decrypt(String s){
  Serial.println ("Starting decryption...") ;
  // Decrypt
  for (byte i = 0 ; i < 16 ; i++)
    iv[i] = my_iv[i] ;
  succ = aes.cbc_decrypt (cipher, check, blocks, iv) ;
}

void setup ()
{
  Serial.begin (9600) ;
  Serial.println ("Starting program...") ;

Serial.println ("this is plain text before encryption...") ;
for (byte i = 0 ; i < 65 ; i++)
 Serial.print(char(plain[i]));

Serial.println ("this is plain text after encryption and decryption...") ;
for (byte i = 0 ; i < 65 ; i++)
 Serial.print(char(check[i]));

Serial.println ("ending encryption...") ;

}

