
/*/image change
let myImage = document.querySelector('#titleImg');

myImage.onclick = function () {
    let mysrc = myImage.getAttribute('src');
    if (mysrc === '/static/Transactions/images/vin icon.png') {
        myImage.setAttribute('src', '/static/Transactions/images/VinAlt.png');
    } else {
        myImage.setAttribute('src', '/static/Transactions/images/vin icon.png');
    }
}*/

/*button to change user name and dynamic welcome message
let myButton = document.querySelector('#changeUserButton');
let myHeading = document.querySelector('#welcomeMessage');

function setUserName() {
    let myName = prompt('Please enter your name.');
    if (!myName) {
        setUserName();
    } else {
        localStorage.setItem('name', myName);
        myHeading.textContent = 'Welcome back, ' + myName;
    }
}

if (!localStorage.getItem('name')) {
    setUserName();
} else {
    let storedName = localStorage.getItem('name');
    myHeading.textContent = 'Welcome back, ' + storedName;
}

myButton.onclick = function () {
    setUserName();
}*/

//amount changer
const amount = document.querySelector('#amountDisplay');

amount.addEventListener('click', updateAmount);

function updateAmount() {
    let newAmount = prompt('Enter a new amount');
    amount.textContent = 'Amount: ' + newAmount + ' EUR';
}