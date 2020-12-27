//click to edit amount
const displayAmount = document.querySelector('#displayDetail');
const editAmount = document.querySelector('#editDetail');
const editButton = document.querySelector('#amountEditButton');

displayAmount.parentNode.removeChild(editAmount);
editButton.addEventListener('click', changeToEdit);

function changeToEdit() {
    displayAmount.parentNode.appendChild(editAmount);
    displayAmount.parentNode.removeChild(displayAmount);
    editAmount.style.margin = 'auto';
}
