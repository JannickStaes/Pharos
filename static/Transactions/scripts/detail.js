//click to edit amount
const amount = document.querySelector('#displayDetail');
const editButton = document.querySelector('#amountEditButton');

editButton.addEventListener('click', editAmount);

function editAmount() {
    amount.textContent = '';
    amount.removeChild(editButton);


}
