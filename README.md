[![keepcached Build Status](https://api.cirrus-ci.com/github/hilbix/keepcached.svg?branch=master)](https://cirrus-ci.com/github/hilbix/keepcached/master)


# keepcached

The idea behind this is to keep a file cached in memory.

However this currently does not work as expected (by me).

Currently following is performing much better:

	keepcached() { while [ -f "$1" ] && printf . && ! read -t .5; do timeout 2 count -fb0 < "$1" >/dev/null; done; }

> This probably needs `/bin/bash`

For `count` see <https://github.com/hilbix/count>.

You can use `/bin/cat` instead, but `count` outputs some additional statistics

