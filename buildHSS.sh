 sudo docker build --target oai-hss --tag oai-hss:production --file component/oai-hss/docker/Dockerfile.ubuntu18.04 component/oai-hss
sudo docker image prune --force
sudo docker image ls
