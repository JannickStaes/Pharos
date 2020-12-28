//click to edit amount
const displayAmount = document.querySelector('#displayAmount');
const editAmount = document.querySelector('#editAmount');
const editButton = document.querySelector('#amountEditButton');

editAmount.parentNode.removeChild(editAmount);
editButton.addEventListener('click', changeToEdit);

function changeToEdit() {
    /*displayAmount.parentNode.appendChild(editAmount);
    displayAmount.parentNode.removeChild(displayAmount);
    editAmount.style.margin = 'auto';*/
}
