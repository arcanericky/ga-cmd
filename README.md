Google Authenticator - Command Line

It's Google Authenticator, but on a command line using your Linux box.

Ever try to sign onto Google only to be asked for a verification code? You sigh as calculate how many calories it'll take you to grab the phone from the other side of the table, or worse, the need to walk to the other side of the room. This program solves that, giving you access to that verification code from your Linux command line.

The extremely small program is built by leveraging the Google Authenticator source itself - specifically, the PAM code. The build script takes your authenticator key (if you don't know how to get this key, then this program is probably not for you), slightly obfuscates it, then creates an executable that can be used to output your current 6 digit verification code every time it is ran. Because a few source modules from Google Authenticator are used, the source for Google Authenticator will be cloned onto your machine. Use the code as is, or use it to build your own large application.

Use:
$ ./build.sh <16 character key>
to build the program which will live at bin/ga-cmd. If the script doesn't work, hack it into submission.

If the executable doesn't spit out a correct verification code (check it against your official Google Authenticator app) then you probably entered the wrong key or the time on your computer is off.

