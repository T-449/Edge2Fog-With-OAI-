sudo docker build --target oai-mme --tag oai-mme:production --file component/oai-mme/docker/Dockerfile.ubuntu18.04 component/oai-mme
sudo docker image prune --force
sudo docker image ls
