
GPanel::GPanel()
{
	owner = this;
}

void GPanel::update()
{
	SetRelative();
	ContainerUpdate();
}

void GPanel::doevents()
{
	if (!visible) return;
	ContainerSendEvents();
}

void GPanel::render()
{
	if (!visible) return;
	ContainerRender();
}