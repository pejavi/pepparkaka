FROM php:7.2.1-fpm-alpine3.7

#RUN apk add --quiet --no-cache g++ icu-dev

RUN docker-php-ext-install sockets

#COPY ./app /var/www/html

#RUN ln -s /var/www/html/vendor/bin/phpunit /bin/phpunit && chmod 0777 /bin/phpunit

WORKDIR /var/www/html
