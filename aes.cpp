#include <bits/stdc++.h>
using namespace std;
typedef bitset<8> byte;
typedef bitset<32> word;

byte keyInBits[4][4];
byte plainTextInBits[4][4];
string keyInEnglish, plainTextInEnglish;
word w[44];

word stateMatrix[4];


byte S_Box[16][16] = {
    {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
    {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
    {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
    {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
    {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
    {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
    {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
    {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
    {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
    {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
    {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
    {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
    {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
    {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
    {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
    {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
};

void calculateKeyInBits()
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            keyInBits[i][j]=(int)(keyInEnglish[i*4+j]);
            //cout << keyInBits[i][j] << endl;
        }
    }
}

void calculatePlainTextInBits()
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            plainTextInBits[i][j]=(int)(plainTextInEnglish[i*4+j]);
            //cout << keyInBits[i][j] << endl;
        }
    }
}

word byteToWord(byte b1, byte b2, byte b3, byte b4)
{
    word ans;
    byte temp;
    for(int i=0;i<32;i++)
    {
        if(i>=0 && i<=7)
        {
            temp=b1;
        }

        else if(i>=8 && i<=15)
        {
            temp=b2;
        }

        else if(i>=16 && i<=23)
        {
            temp=b3;
        }

        else
        {
            temp=b4;
        }
        if(temp.test(i%8))
        {
            ans.set(i);
        }
    }
    return ans;
}


void calculateStateMatrix()
{
    for(int i=0;i<4;i++)
    {
        stateMatrix[i]=byteToWord(plainTextInBits[i][0], plainTextInBits[i][1],
                        plainTextInBits[i][2], plainTextInBits[i][3]);
    }
}

void formSubkeys()
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<32;j++)
        {
            int k=j%8;
            int p=j/8;

            if(keyInBits[i][p].test(k))
            {
                //cout << i << " " << p << " " << k << endl;
                w[i].set(j);
            }
        }
        //cout << w[i] << endl;
    }
}

string binToHex(string s)
{
    if(s=="0000")
       return "0";
    else if(s=="0001")
    return "1";
    else if(s=="0010")
    return "2";
    else if(s=="0011")
        return "3";
    else if(s=="0100")
        return "4";
    else if(s=="0101")
        return "5";
    else if(s=="0110")
        return "6";
    else if(s=="0111")
        return "7";
    else if(s=="1000")
        return "8";
    else if(s=="1001")
        return "9";
    else if(s=="1010")
        return "A";
    else if(s=="1011")
        return "B";
    else if(s=="1100")
        return "C";
    else if(s=="1101")
        return "D";
    else if(s=="1110")
        return "E";
    else if(s=="1111")
        return "F";

}

string byteToHex(byte b)
{
    string s=b.to_string();
    //cout << s;
    string f=s.substr(0, 4);
    string l=s.substr(4, 4);
    return (binToHex(f)+binToHex(l));
}

string wordToHex(word s)
{
    string ans;
    for(int i=0;i<32;i=i+8)
    {
        byte temp;
        for(int j=i;j<i+8;j++)
        {
            if(s.test(j))
            {
                temp.set(j%8);
            }
        }
        //cout << byteToHex(temp) << endl;
        ans=ans+byteToHex(temp);
    }
    return ans;
}

void printKey()
{
    for(int i=0;i<44;i++)
    {
        cout << "w[" << i << "] :" << wordToHex(w[i]) << endl;
    }
}

int hexToInt(char ch)
{
    if(ch>='0' && ch<='9')
    {
        return (int)(ch-'0');
    }

    else
    {
        return (10+(int)(ch-'A'));
    }
}

byte wordToByte(word s, int l)
{
    byte temp;
    for(int i=l*8;i<8*l+8;i++)
    {
        if(s.test(i))
        {
            temp.set(i%8);
        }
    }
    //cout << byteToHex(temp) << " " << l << " " << wordToHex(s) << endl;
    return temp;
}

word g_function(word s)
{
    string h=wordToHex(s);
    //cout << h << endl;
    word ans;
    int ac=0;
    for(int i=0;i<h.length();i=i+2)
    {
        int x, y;
        x=hexToInt(h[i]);
        y=hexToInt(h[i+1]);
        //cout << x << " " << y << endl;
        byte temp=S_Box[x][y];
        //cout << temp << endl;
        for(int j=0;j<8;j++)
        {
            if(temp.test(j))
            {
                ans.set(ac);
            }
            ac++;
        }
        //cout << ans << endl;
    }
    return ans;
}

word leftShift(word s, int steps)
{
    steps=steps%4;
    word temp;

    for(int i=0;i<steps*8;i++)
    {
        if(s.test(i))
        {
            temp.set(i);
        }
    }
    s>>=8*steps;

    for(int i=0;i<steps*8;i++)
    {
        if(temp.test(i))
        {
            s.set(i+(32-steps*8));
        }
    }
    return s;
}

void calculateRoundKey()
{
    for(int i=4;i<44;i=i+4) {
    byte temp;
    word shiftedW3;
    shiftedW3=w[i-1];
    for(int i=0;i<8;i++)
    {
        if(shiftedW3.test(i))
        {
            temp.set(i);
        }
    }

    shiftedW3>>=8;

    for(int i=0;i<8;i++)
    {
        if(temp.test(i))
        {
            shiftedW3.set(i+24);
        }
    }
    //cout << i << ": " << wordToHex(shiftedW3) << endl;
    word g=g_function(shiftedW3);
    word toAdd=((int)pow(2, i/4 -1)%229);
    //cout << ((int)pow(2, i/4 -1)%229) << endl;
    g^=toAdd;
    //cout << "g " << wordToHex(g) << endl;
    w[i]=w[i-4]^g;
    w[i+1]=w[i]^w[i-3];
    w[i+2]=w[i+1]^w[i-2];
    w[i+3]=w[i+2]^w[i-1];
    }
}

void addRoundKey(int roundNumber)
{
    for(int i=0;i<4;i++)
    {
        stateMatrix[i]=stateMatrix[i]^w[4*roundNumber+i];
    }
}

void printStateMatrix()
{
    for(int i=0;i<4;i++)
    {
        cout << wordToHex(stateMatrix[i]) << endl;
    }
}

void substitutionBytes(int roundNumber)
{
    for(int i=0;i<4;i++)
    {
        stateMatrix[i]=g_function(stateMatrix[i]);
    }
}

void shiftRow(int roundNumber)
{
    word r[4];

    for(int i=0;i<4;i++)
    {
        r[i]=byteToWord(wordToByte(stateMatrix[0], i), wordToByte(stateMatrix[1], i),
                        wordToByte(stateMatrix[2], i), wordToByte(stateMatrix[3], i));
        //cout << wordToHex(r[i]) << endl;
    }

    for(int i=0;i<4;i++)
    {
        r[i]=leftShift(r[i], i);
        //cout << wordToHex(r[i]) << endl;
    }

    for(int i=0;i<4;i++)
    {
        stateMatrix[i]=byteToWord(wordToByte(r[0], i), wordToByte(r[1], i),
                        wordToByte(r[2], i), wordToByte(r[3], i));
        //cout << wordToHex(stateMatrix[i]) << endl;
    }
}

int main()
{
    keyInEnglish="Thats my Kung Fu";
    plainTextInEnglish="Two One Nine Two";
    calculateKeyInBits();
    calculatePlainTextInBits();
    formSubkeys();
    calculateRoundKey();
    calculateStateMatrix();
    addRoundKey(0);
    //printStateMatrix();
    substitutionBytes(1);
    //printStateMatrix();
    shiftRow(1);
    printStateMatrix();

    //printKey();
}

