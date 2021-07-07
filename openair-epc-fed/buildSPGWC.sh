sudo docker build --target oai-spgwc --tag oai-spgwc:production --file component/oai-spgwc/docker/Dockerfile.ubuntu18.04 component/oai-spgwc
sudo docker image prune --force
sudo docker image ls
