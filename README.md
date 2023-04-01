
# Text Encrypter

This program was made to explore ways that data might be encrypted. To use it, you can build the code with this command:   
    ```
    gcc -o enc main.c
    ```
The usage is as follows:   
    ```
    ./enc -d/e (-p [password]) [input]
    ```
To encrypt the string "A little piece of text." with the password "pswd", you can use the following command:   
    ```
    ./enc -ep "pswd" "A little piece of text."
    ```
It produces this string: "&X8eA?uk;LTHPkd~{Y-aWqH"   
To decrypt it again, you can run this command:
    ```
    ./enc -dp "pswd" "&X8eA?uk;LTHPkd~{Y-aWqH"
    ```
