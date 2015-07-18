# fuzzyjs
This is a fully embeddable javascript interpreter written in c++. It is not ECMAScript compliant, is not efficient, probably shouldn't be used in a real situation. The benefit, however, is that this project has _absolutely no dependencies._ You do not even have to include stdio.h! The only requirement is that you implement **malloc** and support of the **new** keyword.

# Why is this a thing?
I want to utilize javascript in a baremetal project of mine. As such, I don't have access to any of the standard libraries which is an odd restriction. I couldn't find any project that was completely standalone, so I decided to try my hand at rolling my own. The scope of fuzzyjs is not overly ambitious and I do not expect to ever support full ECMAScript compliance. But who knows?