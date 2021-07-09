Install the following in the venv of OIDC_Client and Proxy2:
	django==2.2.12
	django-oidc-provider


PRE-DEPLOYMENT:

1) Provide IP of your device in openair-epc-fed/component/oai-mme/src/nas/emm/nas_emm_task.c: 164 (server.sin_addr.s_addr = inet_addr("IP of your device"))

2) Build the MME using ./buildMME.sh from within openair-epc-fed directory

3) Build the HSS using ./buildHSS.sh from within openair-epc-fed directory

4) Provide IP of your device in Proxy2/django-oidc-provider/example/receiverReply.py: 9 (vUser_IP = 'IP of your device')




DEPLOYMENT:

1) Run the following from within openair-epc-fed directory in the specific  order as below-
	a) ./deployNetwork.sh
	b) ./deployCassandra.sh
	c) ./deployHSS.sh
	d) ./deployCassandra.sh

2) Run all receiverReply.py and receiverRequest.py files in OIDC_Client and Proxy2

3) Run tempUE from OIDC_Client

4) Run ./resCalculator from within ResCalculator directory

5) Run django server at port 9999 for OIDC_Client

6) Run django server at default port(8000) for Proxy2

7) Go to localhost:9999/client from browser

8) If you want to login to django admin use Username: admin, Password:admin


OBSERVATION:

The login button in the localhost:9999/client page will be disabled. Press 's' in the temporary UE. Once the UE is authenticated the login button will be enabled in the same page. After pressing login you will be authenticated to the fog using OIDC.
