[![keepcached Build Status](https://api.cirrus-ci.com/github/hilbix/keepcached.svg?branch=master)](https://cirrus-ci.com/github/hilbix/keepcached/master)


# keepcached

The idea behind this is to keep a file cached in memory.

However this currently does not work as expected (by me).

Currently following is performing much better:

	while sleep .5; do timeout 2 count -fb0 <$FILE >/dev/null; done

For `count` see <https://github.com/hilbix/count>

