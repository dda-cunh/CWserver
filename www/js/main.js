const g_address = "http://localhost:8080/";

$(document).ready(function()
{
	$('.owl-carousel').owlCarousel(
	{
		loop: true,
		margin: 20,
		responsiveClass: true,
		responsive:
		{
			0:
			{
				items: 1
			},
			600:
			{
				items: 2
			},
			1000:
			{
				items: 3
			}
		}
	});

	$('#testimonialsCarousel').carousel(
	{
		interval: 3000
	});
});

function addToCart(id)
{
	$.ajax(
	{
		method: "POST",
		url: g_address + "add_cart",
		data: {item_id: id},
		success: function(data)
		{
			$('#cartCount').text(data);
		}
	});
}
