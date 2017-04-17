### Google Authenticator - Command Line

It's like Google Authenticator, but on a command line using your Linux box.

![ga-cmd](https://raw.github.com/wiki/arcanericky/ga-cmd/images/ga-cmd.png "ga-cmd execution")

Ever try to sign onto Google only to be asked for a verification code? You sigh as calculate how many calories it'll take you to grab the phone from the other side of the table, or worse, the need to walk to the other side of the room. This program solves that, giving you access to that verification code from your Linux command line.

The extremely small program is built by leveraging the Google Authenticator source itself - specifically, the PAM code. The build script takes your authenticator key, slightly obfuscates it, then creates an executable that can be used to output your 6 digit verification code every time it is executed.

**Warning: Once you've built this program, protect it. It contains your authentication seed (though it's loosely obscured) and anyone that can execute it can use your verification codes.**

#### Background
The Google Authentication codes are standardized message authentication codes called HMACs. Read more about them starting at the [Wikipedia page for HMAC](https://www.google.com "Wikipedia: HMAC"). You can find more source code for generating HMACS by searching here on [GitHub for HMAC](https://github.com/search?q=hmac "GitHub: HMAC"). This project was done for the fun of bending Google's PAM code to generate an HMAC.

#### Building
The git repository contains submodules. Clone this repo then setup the submodules with:
```
$ git clone https://github.com/arcanericky/ga-cmd.git
$ git submodule init
$ git submodule update
```
Once you've done this, switch into the src directory and execute build.sh
```
$ cd src
$ ./build.sh <16 character seed>
```
The source will compile, a test routine is built and verified, then the ga-cmd is built and deposited at bin/ga-cmd.

#### Using
Simply run the executable and it will emit a 6 digit verification code:
```
$ bin/ga-cmd
123456
```
Check the verification code against your official Google Authenticator app. If it's wrong, check the time on your computer (it's used to calculate the verification code). If the time on the computer doesn't match the time on your phone, sync them and try again. If it still doesn't work, odds are you entered the wrong 16 digit seed when you ran build.sh

If the executable doesn't emit a correct verification code (check it against your official Google Authenticator app) then you probably entered the wrong key or the time on your computer is off.

Even better, pipe its output to a clipboard utility such as xclip and paste the result into the verification code entry box.
```
$ bin/ga-cmd | xclip -sel clipboard
```
Obviously, that's a decent amount of typing so you'd want to script it so you can step through the whole dance by just clicking your mouse.
