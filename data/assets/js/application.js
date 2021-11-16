//**Le fichier JS principale de la page de l'application*/
/**
 * @author Daniel Boisclair
 * @version 1.0
 * @modifications
 *              Aucune en date du 03 Septembre 2021
 */

/** Liste du bois */
var listeBoisInformations = {};

const routeAPI = "http://172.16.203.86/";

/**Fonction qui permet d'envoyer une requête par la méthode GET */
function sendGETRequest(apiRequest, params, callback) {
    /*let paramsString = "/";
    params.forEach(param => paramsString += param + "/");*/

    let paramsString = "?";
    for (const [key, value] of Object.entries(params)) {
        paramsString += key + "=" + value + "&";
    }
    paramsString = paramsString.slice(0, -1);

    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = callback;
    xhr.open("GET", routeAPI + apiRequest + paramsString, true);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.setRequestHeader('Access-Control-Allow-Headers', '*');
    xhr.setRequestHeader('Access-Control-Allow-Origin', '*');
    xhr.send();
}

/**Fonction qui permet d'obtenir un élément par le query du document */
function getElement(query) {
    return document.querySelector(query);
}

/**Fonction qui affiche les détails du bois */
function afficherDetailsBois(idBois) {
    /*sendGETRequest("ObtenirBois", {"idBois" : idBois}, function() {
        if (this.readyState != 4) return;
        if (this.status == 200) {
            var bois = JSON.parse(this.responseText);
            if (bois) {
                getElement("#bois").innerHTML = bois.nom;
                getElement("#type").innerHTML = bois.type;
                getElement("#origine").innerHTML = bois.origine;
                getElement("#tempsSechage").innerHTML = bois.tempsSechage;
                getElement("#tempMin").innerHTML = bois.tempMin;

                getElement("#typeBois").innerHTML = bois.nom;
                getElement("#tempMinFour").innerHTML = bois.tempMin;
                getElement("#tempsTotal").innerHTML = bois.tempsSechage;
            } else {
                console.log("err");
            }
        }
    });*/
    let bois = listeBoisInformations[idBois];
    getElement("#bois").innerHTML = bois.nom;
    getElement("#type").innerHTML = bois.type;
    getElement("#origine").innerHTML = bois.origine;
    getElement("#tempsSechage").innerHTML = bois.tempsSechage;
    getElement("#tempMin").innerHTML = bois.tempMin;

    getElement("#typeBois").innerHTML = bois.nom;
    getElement("#tempMinFour").innerHTML = bois.tempMin;
    getElement("#tempsTotal").innerHTML = bois.tempsSechage;

}

/**Event du chargement de la page */
window.onload = function() {
    /**L'élément select */
    var selectionBois = document.querySelector("#selectionBois");

    /**Fonction qui permet d'obtenir tous le bois dans la BDD */
    sendGETRequest("getAllWoodOptions", {}, function() {
        if (this.readyState != 4) return;
        if (this.status == 200) {
            var listeBois = JSON.parse(this.responseText);
            if (listeBois) {
                listeBois.forEach(function(bois) {
                    selectionBois.innerHTML += ("<option value='" + bois.id + "'>" + bois.nom +"</option>");

                    listeBoisInformations[bois.id] = bois;
                });

                afficherDetailsBois(listeBois[0].id);
            } else {
                console.log("err");
            }
        }
    });

    /**Fonciton qui permet de rafraichir les données du bois sélectionné */
    selectionBois.addEventListener("change", function() {
        afficherDetailsBois(this.value);
    });

    //Update la température du four à chaque seconde
    setInterval(function() {
        sendGETRequest("getFourTemp", {}, function() {
            if (this.readyState != 4) return;
            if (this.status == 200) {
                var tempFour = JSON.parse(this.responseText);
                if (tempFour != null) {
                    let tempElement = getElement("#tempVal");
                    tempElement.innerHTML = tempFour;
                } else {
                    //console.log("err");
                }
            }
        });
    }, 2000);
}