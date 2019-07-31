# Google Authenticator - Command Line

It's like Google Authenticator, but on a command line using your Linux box.

[![Build Status](https://travis-ci.com/arcanericky/ga-cmd.svg?branch=master)](https://travis-ci.com/arcanericky/ga-cmd)
[![codecov](https://codecov.io/gh/arcanericky/ga-cmd/branch/master/graph/badge.svg)](https://codecov.io/gh/arcanericky/ga-cmd)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com)

```
$ ga-cmd
123456

$ ga-cmd my-fave-site
654321
```

## The Better Alternative

If you're using this repository for the compiled releases to generate TOTP codes you should instead be using [my TOTP utility](https://github.com/arcanericky/totp) written in Go. It offers too many improvements to be overlooked and is the utility I personally use. Among these are:
- Executables for Linux, Windows, and Macintosh (more on request)
- Support for multiple secrets
- Command line options to add, delete, update, rename, and list secrets
- No more manual editing of a configuration file
- Ad-hoc secret support
- Bash completion
- Specify the time used to generate the seed, and/or adjust the current time forward and backward
- Continuous code output every 30 seconds
- Streaming of a shared secret collection using stdio

My [TOTP project on GitHub](https://github.com/arcanericky/totp).

## What It Does
Ever try to sign onto Google only to be asked for a verification code? You sigh as calculate how many calories it'll take you to grab the phone from the other side of the table, or worse, the need to walk to the other side of the room. This application solves that, giving you access to that verification code from your Linux command line.

The extremely small application is built by leveraging the [Google Authenticator PAM Module](https://github.com/google/google-authenticator-libpam) source itself. The build script takes your authenticator key, slightly obfuscates it, then creates an executable that can be used to output your 6 digit verification code every time it is executed.

## Compatibility
This application outputs TOTP codes. These are compatible with a number of services include [**Google**](https://accounts.google.com/signin), [**GitHub**](https://github.com/login), [**Dropbox**](https://www.dropbox.com/en_GB/login), [**AWS**](https://aws.amazon.com/marketplace/management/signin), and probably any other service that implements TOTP codes. If you use this application for a service not listed here, please let me know or file an [Issue](https://github.com/arcanericky/ga-cmd/issues) or [Pull Request](https://github.com/arcanericky/ga-cmd/pulls) to add it.

## Quick Start
- [Download](https://github.com/arcanericky/ga-cmd/releases) the current release of `ga-cmd`
- Make it executable: `chmod 755 ga-cmd`
- Create and populate a `$HOME/.ga-cmd` file: `echo myfavesite=thisismyexamplekey > $HOME/.ga-cmd`
- Set the configuration file permissions: `chmod 600 $HOME/.ga-cmd`
- ...and execute: `./ga-cmd myfavesite`

### Background
These authentication codes are standardized message authentication codes derived from the Time-based One-Time Password algorithm (TOTP). Read more about them starting at the [Wikipedia page for TOTP](https://en.wikipedia.org/wiki/Time-based_One-time_Password_algorithm). You can find more source code for generating TOTPs by searching here on [GitHub for TOTP](https://github.com/search?q=totp). This project initially started for the fun of bending Google's PAM code to generate a TOTP code.

### Configuration File
Multiple keys can be managed through a configuration file that resides at `$HOME/.ga-cmd`. The format is one key per line, each line containing the key name followed by `=` followed by the key. No spaces around the `=`. The general format is:
```
name=key
```
An example file would be
```
google=1234567890123456
github=0011223344556677
dropbox=99887766554433221100aa
```
This configuration file contains your keys which must be kept private. **Do not share this file or its contents.** Because this file contains sensitive information, the application will not use it if the permissions are not restrictive enough and give an error reflecting this. The file should be owner read/write only. Group and Other/World should not have access. A file with proper permissions will look like
```
ls -l ~/.ga-cmd
-rw------- 1 username username 114 Apr 20 13:37 /home/username/.ga-cmd
```
Issuing the command `chmod 600 ~/.ga-cmd` apply the proper permissions.

### Using
If the application was built with an internal key, running the executable will emit a 6 digit verification code for the key:
```
$ bin/ga-cmd
123456
```

Running the application with a single argument will cause it to ignore the internal key (if built with one), lookup the key in the key configuration file, then generate a verification code based on the key. If the applicaton was not built with an internal key, the first key in the configuration file will be used. For example, using the above configuration file to generate a verification code for the entry named `github` would be
```
$ bin/ga-cmd github
654321
```
Note these lookups are **case sensitive**.
```
$ bin/ga-cmd GITHUB
Could not find key for GITHUB in /home/username/.ga-cmd.
```

Check the verification code against whichever application you normally used to generate TOTP codes, such as [Google Authenticator](https://play.google.com/store/apps/details?id=com.google.android.apps.authenticator2) or [Authy](https://authy.com/). If it's wrong, check the time on your computer (it's used to calculate the verification code). If the time on the computer doesn't match the time on your phone, sync them and try again. If it still doesn't work, odds are you entered an incorrect key when you ran `build.sh`.

Even better, pipe the output to a clipboard utility such as `xclip` and paste the result into the verification code entry box.
```
$ bin/ga-cmd | xclip -sel clipboard
```
Obviously, that's a decent amount of typing so you'd want to script it so you can step through the whole dance by just clicking your mouse.

### Command Line Flags

Two command line flags are supported

#### Version

Use the `--version` or `-v` flag to show the version of `ga-cmd`. This is useful for version validation and bug reporting. Example:
```
$ ga-cmd --version
ga-cmd version 1.0.0
```

#### List Tags
Use the `--list` or `-l` flag to list the key names (tags) in the configuration file. Example:
```
$ ga-cmd --list
myfavesite
mysocialsite
myfinancialsite
```

### Bash Completion

Bash completions can be performed on the command line with the Bash completion script `ga-cmd-completion.sh`. Example:
```
$ ga-cmd <tab><tab>
myfavesite myfinancialsite mysocialsite
$ ga-cmd myf<tab><tab>
myfavesite myfinancialsite
```

And using
```
$ ga-cmd mys<tab><tab>
```
will autocomplete to
```
$ ga-cmd mysocialsite
```

To enable Bash completion, before executing `ga-cmd` use
```
source ga-cmd-completion.sh
```

Or to enable Bash completion for all future terminals add the above to your `.bashrc` file.

### Building

**Warning: If you build this application with an internal key, protect the executable and don't distribute it. It contains your authentication key which must be kept private (though it's loosely obscured), and anyone that can execute the application can use your verification codes.**

The git repository contains submodules. Clone this repo then setup the submodules with:
```
$ git clone https://github.com/arcanericky/ga-cmd.git
$ cd ga-cmd
$ git submodule update --init --recursive
```
Install the PAM development library. On Ubuntu do this with:
```
$ sudo apt-get install -y libpam0g-dev
```
**Optional**: If you want to generate code coverage reports using the makefile `coverage` target, install [gcovr](http://www.gcovr.com).
```
$ sudo apt-get install -y gcovr
```
Once you've done this, switch into the `src` directory and execute `build.sh`. If you want the application to contain a single internal key for a single service, specify the key on the command line. Otherwise, just execute `build.sh` and use the external configuration file.

Building without an internal key:
```
$ cd src
$ ./build.sh
```
Building with an internal key:
```
$ cd src
$ ./build.sh <16 to 64 character key>
```

The source will compile, test routines are built and verified, then the `ga-cmd` is built and deposited at `bin/ga-cmd`.
