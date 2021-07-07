sudo docker build --target oai-spgwu-tiny --tag oai-spgwu-tiny:production --file component/oai-spgwu-tiny/docker/Dockerfile.ubuntu18.04 component/oai-spgwu-tiny
sudo docker image prune --force
sudo docker image ls

