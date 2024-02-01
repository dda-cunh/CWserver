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
  
window.addEventListener('load', () =>
{
	const currentUrl = window.location.href;
	const newUrl = currentUrl.replace(/\.html$/, '');

	if (newUrl !== currentUrl)
		history.replaceState({}, '', newUrl); 
});