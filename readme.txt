Installing docker and downloading images:

	// If you have docker pre-installed please ensure that it is the same version as the one mentioned in step (g).

	a) Initially create a docker account

	b) sudo apt-get remove docker docker-engine docker.io containerd runc

	c) sudo apt-get update

	d) sudo apt-get install apt-transport-https ca-certificates curl gnupg lsb-release

	e) curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

	f) echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

	g) sudo apt-get install docker-ce=5:19.03.6~3-0~ubuntu-bionic docker-ce-cli=5:19.03.6~3-0~ubuntu-bionic containerd.io

	h) sudo usermod -a -G docker <yourUsername>

	i) sudo docker run hello-world // just to test if docker is installed and working properly

	j) sudo docker login  // login to your docker account.

	k) sudo docker pull ubuntu:bionic

	l) sudo docker pull cassandra:2.1

	m) sudo docker logout


Setting up network:

	a) sudo sysctl net.ipv4.conf.all.forwarding=1

	b) sudo iptables -P FORWARD ACCEPT


Downloading and configuring OAI:

	a) git clone https://github.com/OPENAIRINTERFACE/openair-epc-fed.git

	b) cd openair-epc-fed

	c) git checkout 2021.w22

	d) git checkout master

	e) ./scripts/syncComponentsLegacy.sh

	f) sudo chmod ugo+rwx buildMME.sh buildHSS.sh buildSPGWC.sh buildSPGWU.sh deployNetwork.sh deployCassandra.sh deployHSS.sh deployMME.sh

	g) Run buildMME.sh, buildHSS.sh, buildSPGWC.sh, buildSPGWU.sh and wait for completion	// deploying SPGWC and SPGWU is not necessary for this implementation but you can choose to build them anyway

	h) Run deployNetwork.sh, deployCassandra.sh, deployHSS.sh, deployMME.sh in the specific order	// see if everything has connected properly without issues

	Issues:
		a) If deployCassandra.sh shows error run again
		b) If deployHSS.sh shows that it can't find IP 192.168.62.2 a restart helped in my case

	// If everything has connected properly proceed for the next steps.

	i) Copy filestochange/simpleServer.c to openair-epc-fed/component/oai-mme/src/mme_app directory

	j) Copy and replace filestochange/CMakeLists.txt in openair-epc-fed/component/oai-mme/src/mme_app directory

	k) Provide IP of your device in filestochange/nas_emm_task.c: 164 (server.sin_addr.s_addr = inet_addr("IP of your device"))	// the line number may not match accurately

	l) Copy and replace filestochange/nas_emm_task.c in openair-epc-fed/component/oai-mme/src/nas/emm directory

	m) Copy and replace filestochange/oai_mme.c in openair-epc-fed/component/oai-mme/src/oai_mme directory

	n) Copy and replace filestochange/tasks_def.h in openair-epc-fed/component/oai-mme/src/common directory

	o) Provide IP of your device in Proxy2/django-oidc-provider/example/receiverReply.py: 9 (vUser_IP = 'IP of your device')	// the line number may not match accurately

	p) Run buildMME.sh and wait for completion	// it may seem stuck at some point but its not

	q) Run deployMME.sh


Deployment:

	// Install django==2.2.1, django-oidc-provider in the venv of OIDC_Client and Proxy2. Please use the specific version of django

	a) Run all receiverReply.py and receiverRequest.py files in OIDC_Client and Proxy2

	b) Run tempUE from OIDC_Client

	c) Run resCalculator from within ResCalculator directory

	// if you use venv run python manage.py runserver from within venv and from the directory containing manage.py file

	d) Run django server at port 9999 for OIDC_Client	// python manage.py runserver 9999

	e) Run django server at default port(8000) for Proxy2	// run python manage.py runserver from within Proxy2/django-oidc-provider/example directory

	f) Go to localhost:9999/client from browser

	g) If you want to login to django admin use Username: admin, Password:admin


Post-deployment:

The login button in the localhost:9999/client page will be disabled. Press 's' in the temporary UE. Once the UE is authenticated the login button will be enabled in the same page. After pressing login you will be authenticated to the fog using OIDC.

For running the emulation again go to localhost:9999/client page from the browser. The login button will be disabled. Rerun deployMME.sh
