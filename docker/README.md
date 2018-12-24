
# Docker boilerplate

### Folder structure
`./app` - folder where to store project's code  
`./data` - this is a place where data of services is being stored (mysl data, redis cache etc)   
`./docker` - all config files for each of services divided per environment  

### How to run this?

1. Get started
`apt install docker docker-compose`

2. Create network
`docker network create nginx-proxy`

3. Start nginx proxy server
`docker run -d -p 80:80 --restart always --name proxy --network nginx-proxy -v /var/run/docker.sock:/tmp/docker.sock:ro jwilder/nginx-proxy:alpine`

4. Build app
`docker-compose build`

4. Start app
`docker-compose up -d --force-recreate`
