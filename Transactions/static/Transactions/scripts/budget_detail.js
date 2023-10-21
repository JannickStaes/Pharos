//click to edit Budget
const displayBudget = document.querySelector('#displayBudget');
const editBudget = document.querySelector('#editBudget');
const editButton = document.querySelector('#budgetEditButton');
const newBudget = document.querySelector('#newBudget')

displayBudget.parentNode.removeChild(editBudget);
editButton.addEventListener('click', changeToEdit);

function changeToEdit() {
    displayBudget.parentNode.appendChild(editBudget);
    displayBudget.parentNode.removeChild(displayBudget);
    editBudget.style.margin = 'auto';
}

const currentURL = window.location.href;
const url = currentURL + '/update_target/'; 
const data = {
  new_budget: newBudget,
};

fetch(url, {
  method: 'POST',
  headers: {
    'Content-Type': 'application/json', // Adjust the content type as needed
    // You may need to include additional headers like authentication tokens
  },
  body: JSON.stringify(data)
})
  .then(response => response.json()) // Parse the response as JSON
  .then(data => {
    console.log(data); // Do something with the data
  })
  .catch(error => {
    console.error('Error:', error);
  });

  