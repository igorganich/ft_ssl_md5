# ft_ssl_md5
## ft_ssl_md5
ft_ssl_md5 is a 42 project that aims to rewrite ```md5``` and ```sha256``` message digest algorithms. 
## Download and comlipe this project
```
git clone https://github.com/superuzer/ft_ssl_md5.git ~/ft_ssl_md5
cd ~/ft_ssl_md5
make
```
## Usage
```ft_ssl command [command opts] [command args]```

```ft_ssl``` supports next commands of original ```md5``` commands : ```[-pqrs]```

## Examples
```
echo "sometext" | ./ft_ssl md5 -p -s "112" -rs "smth" -qs "" /etc/networks
sometext
ee321721ddf85e01b4cff48b4fee3c08
MD5 ("112") = 7f6ffaa6bb0b408017b62254211691b5
56963dd5cdbc8011393e85a0ca2f8875 "smth"
d41d8cd98f00b204e9800998ecf8427e
96db4c4794e603726d38c5b03a7f7e98
```
