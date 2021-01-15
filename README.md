# Inetnav
(C++/MFC/Win32/Windows 3.1/Visual C++ 1.0/1.52c) Integrated Internet access environment for Finger, WHOIS, Gopher, NNTP, SMTP, FTP, POP/SMTP Email protocols -- written when I was 15 years old!  There is no HTTP/Web browsing support as I figured that was pointless due to the Browser Wars going on at the time. If you wanted to surf the web, I reckoned, just use your browser that should also be installed.

# Inetnav
(C++/MFC/Win32/Windows 3.1/Visual C++ 1.0/1.52c) Integrated Internet access environment for Finger, WHOIS, Gopher, Network News Transfer Protocol (NNTP, aka Usenet News), SMTP, FTP, Internet Relay Chat (or IRC for short), POP/SMTP Email, and HTTP protocols -- written when I was 15 years old and completely self-taught (well, I hadn't even graduated high school yet, so...you know...didn't have a degree and they didn't teach coding to kids a whole lot back then)!

**NOTE:** Much of this code won't work because you can't configure it anymore.  I mean, who runs NNTP or IRC servers these days?  Pretty much no one.  The Gopher protocol is all but dead and gone.  Finger is anathema because of the Cybersecurity threat posed by running a Finger service, and WHOIS servers are also likewise very rare, since there are sites such as [GoDaddy](https://www.godaddy.com), [Network Solutions](https://www.networksolutions.com) etc. where you can now retrieve that information.

**Credit where credit is due** I would not have been able to produce this software had it not been for the _wonderful_ programming book called _Building Internet Applications with Visual C++_ by Kate Gregory, Paul Robichaux, Brady Merkel, and Markus Pope (Que Corporation, 1995).  

Literally this book was _THE_ source book on programming the Internet with Windows and the Microsoft Foundation Class Library prior to the advent of other tomes and future versions of the Visual C++ Development environment.  (Back then there was no [Microsoft Visual Studio](https://visualstudio.microsoft.com/).

## Development Environemnt/Framework

This software was compiled using Microsoft Visual C++ 1.52c and the Microsoft Foundation Class Library v1.0/2.0.

Requires Internet Explorer 6.0 or earlier and can work on Microsoft Windows 3.1 or Microsoft Windows 95 ("Chicago").

## Problem this software solved

Back in 1994-1995, accessing the Internet was new -- at least for Everyman.  

Back then, you'd subscribe to a service provided by a company known as an Internet Service Provider or ISP.  

This was when devices known as _modems_ (short for _modulator/demodulator_) were utilized to allow computers to communicate over the Internet using a phone line.

When we subscribed to Internet Service Proviers or ISPs back in the day, we received usually bundled suites of separate software programs for each mode you wanted to use the Internet (there was not just a web browser where you read your email, downloaded files, chatted with people on forums etc -- but actually a sepaate program for each of those).

I thought that was stupid.  I wanted a software program that would allow me to utilize every type of Internet service under one roof, with easy navigation and file management.

I was quite taken with America Online (AOL)'s desktop user-interface app back in that day, and I sought to make my user interface be patterned after that.

To launch the software, you connect your modem to your ISP and then you launch the program by clicking its icon in Program Manager.

THen you are taken to large application window such like Microsoft Word.  The large window has several smaller windows that appear inside it.  These are known as "areas."

The Main Menu is the initial "area" you see.  On the Main Menu are nine big, friendly picture buttons that take you to the areas of the program that allow access to the various Internet services, and/or online Help (which I also authored) and option settings.  THe menu and toolbar wuold change dynamically, depending on which area's window currently had the focus.  A full set of online Help documentation was created for the program, as well as a bug reporting and feedback facility.

To compile this code now, you would need to set up a Virtual Machine, say in OracleVM VirtualBox and/or VMWare Workstation.  The OS needs to be MS DOS 6.22, and you need Microsoft Windows 3.1 or higher installed. I do not recall offhand how much hard disk space, but to even compile this code will require the installation of the Microsoft Visual C++ 1.52c development environment on the virtual machine.  Back in the day, it was $80 at [CompUSA](https://en.wikipedia.org/wiki/CompUSA) and it came in a big cardboard box with a CD-ROM and really thick paper reference manuals.

Enjoy!
