# warp2
Email / messaging with encrypted headers and metadata.  

## What?  How?  Why?

I had a great intern working for me one summer several years ago - Hanna Bjorgvinsdottir - and we spent some time talking about computing projects.  I don't remember how this came up, but we talked about the metadata crawling that was going on with our benevolent leaders.  It didn't matter what you wrote in an email, you would be statistically correlated with someone due to the time and frequency of your communications with them.  Even if the content was not known, because it was encrypted, if they were into "dodgy stuff" you could be implicated.

This actually happened to a friend of mine.  He went to a wedding and a friend of a friend at the wedding was a drug dealer.  His next visit to the USA was not so smooth, and my friend had done nothing wrong.

So, what if you could send a message to someone, where the metadata was encrypted?  So an eavesdropper did not know who sent the message and to whom?  How does the computer know where to send it, in that case?  Well, it doesn't.

warp2 works by encryping the header (to, from, date) as a separate piece of data to the message.  It just uses regular public key encryption, so you need to swap keys with your friends to communicate as usual.  The header also contains identifying information to locate the message in the database.  There is one inbox shared with all users (obviously this does not scale too much).  The client attempts to decrypt this small header info of each message.  If the decryption works, then the message is for you, and the message body is found, downloaded, and decrypted.  If the decryption does not work, then the message is not for you and you can flag it as ignored.  Of course, to improve privacy the client can occasionally grab random messages and just dump them to /dev/null, but the code doesn't do that yet.

We wrote this as a paper preprint (https://arxiv.org/abs/1411.6409) and this code is the proof of concept work to go with that paper.  It's BSD license, so if someone finds this useful then just help yourself :)
