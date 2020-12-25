//amount changer
const amount = document.querySelector('#amountDisplay');

if (typeof amount !== 'undefined' && amount != null) {
    amount.addEventListener('click', updateAmount);
}

function updateAmount() {
    let newAmount = prompt('Enter a new amount');
    amount.textContent = 'Amount: ' + newAmount + ' EUR';
}