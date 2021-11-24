//**Le fichier JS principale de la page de connexion */
/**
 * @author Daniel Boisclair
 * @version 1.0
 * @modifications
 *              Aucune en date du 03 Septembre 2021
 */

/**Événement du chargement de la page */
window.onload = function() {
    //window.history.replaceState(null, "", getAuthURL());

    /**Form de la connexion */
    let connexionForm = document.getElementById("connexionForm");
    connexionForm.onsubmit = function() {
        let formData = new FormData(connexionForm);
        let data = {
            courriel: formData.get("courriel"),
            motDePasse: formData.get("motDePasse")
        };

        var xhr = new XMLHttpRequest();
        /**Fonction qui permet de connecter l'utilisateur */
        xhr.onreadystatechange = function () {
            if (this.readyState != 4) return;
            console.log(this.responseText);
            if (this.status == 200) {
                var response = JSON.parse(this.responseText);
                window.location.replace("application");
                localStorage.setItem("token", response.token);
            }
        };

        /**Informations pour l'envoie d'une requête de l'API */
        xhr.open("POST", "http://10.0.0.78:8000/api/Connexion", true);
        //xhr.withCredentials = true;
        xhr.setRequestHeader('Content-Type', 'application/json');
        /*xhr.setRequestHeader('Access-Control-Allow-Headers', '*');
        xhr.setRequestHeader('Access-Control-Allow-Origin', '*');*/
        xhr.send(JSON.stringify(data));

        return false;
    }
};